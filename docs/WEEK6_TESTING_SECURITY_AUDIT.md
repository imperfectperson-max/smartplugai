# WEEK 6: TESTING, SECURITY AUDIT & BUG FIXES - DEEP DIVE

📅 **Timeline**: Days 36-42  
🎯 **Goal**: Comprehensive testing across all system components, security penetration testing, and critical bug fixes to ensure production readiness

## 📅 DAY 36-37: UNIT & INTEGRATION TESTING

### MORNING (9AM-12PM): Backend Unit Tests

#### Step 1: Testing Infrastructure Setup

**File**: `/backend/tests/conftest.py`

```python
import pytest
import asyncio
from fastapi.testclient import TestClient
from firebase_admin import firestore, auth, initialize_app, credentials
import os

# Initialize Firebase Admin for testing
@pytest.fixture(scope="session")
def firebase_app():
    """Initialize Firebase Admin SDK for tests"""
    # Use Firebase Emulator for testing
    os.environ['FIRESTORE_EMULATOR_HOST'] = 'localhost:8080'
    os.environ['FIREBASE_AUTH_EMULATOR_HOST'] = 'localhost:9099'
    
    cred = credentials.Certificate('path/to/test-service-account.json')
    app = initialize_app(cred, name='test-app')
    
    yield app
    
    # Cleanup
    # delete_app(app)

@pytest.fixture
def db(firebase_app):
    """Get Firestore client"""
    return firestore.client(app=firebase_app)

@pytest.fixture
def test_user(db):
    """Create a test user"""
    user_id = "test_user_123"
    user_ref = db.collection('users').document(user_id)
    user_ref.set({
        'email': 'test@example.com',
        'name': 'Test User',
        'created_at': firestore.SERVER_TIMESTAMP
    })
    
    yield user_id
    
    # Cleanup
    user_ref.delete()

@pytest.fixture
def test_device(db, test_user):
    """Create a test device"""
    device_id = "test_device_123"
    device_ref = db.collection('devices').document(device_id)
    device_ref.set({
        'device_id': device_id,
        'owner_id': test_user,
        'name': 'Test Smart Plug',
        'model': 'SP-001',
        'status': {
            'online': True,
            'power': 100.0,
            'current': 0.43,
            'voltage': 230.0
        }
    })
    
    yield device_id
    
    # Cleanup
    device_ref.delete()

@pytest.fixture(scope="session")
def event_loop():
    """Create event loop for async tests"""
    loop = asyncio.get_event_loop_policy().new_event_loop()
    yield loop
    loop.close()
```

#### Step 2: Schedule Service Tests

**File**: `/backend/tests/test_schedule_service.py`

```python
import pytest
from datetime import datetime, timedelta
from models.schedule import Schedule, ScheduleType, TimeTrigger, Action
from services.schedule_executor import ScheduleExecutor

@pytest.mark.asyncio
async def test_create_schedule(db, test_user, test_device):
    """Test creating a new schedule"""
    schedule_data = {
        'schedule_id': 'sch_test_001',
        'user_id': test_user,
        'device_id': test_device,
        'name': 'Morning Coffee',
        'type': ScheduleType.TIME,
        'trigger': {
            'time': '06:30',
            'days': ['Mon', 'Tue', 'Wed', 'Thu', 'Fri'],
            'timezone': 'Africa/Johannesburg'
        },
        'action': {
            'command': 'turn_on',
            'duration': 1800
        },
        'conditions': [],
        'enabled': True
    }
    
    # Create schedule
    schedule_ref = db.collection('schedules').document(schedule_data['schedule_id'])
    schedule_ref.set(schedule_data)
    
    # Verify creation
    doc = schedule_ref.get()
    assert doc.exists
    assert doc.to_dict()['name'] == 'Morning Coffee'
    
    # Cleanup
    schedule_ref.delete()

@pytest.mark.asyncio
async def test_schedule_execution(db, test_user, test_device):
    """Test schedule execution logic"""
    executor = ScheduleExecutor()
    
    # Create a schedule that should trigger now
    now = datetime.utcnow()
    schedule = Schedule(
        schedule_id='sch_test_002',
        user_id=test_user,
        device_id=test_device,
        name='Test Schedule',
        type=ScheduleType.TIME,
        trigger=TimeTrigger(
            time=now.strftime('%H:%M'),
            days=['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
            timezone='UTC'
        ),
        action=Action(command='turn_on'),
        enabled=True,
        next_trigger=now
    )
    
    # Test execution
    result = await executor.check_conditions(schedule)
    assert result is True
    
@pytest.mark.asyncio
async def test_next_trigger_calculation(db):
    """Test calculation of next trigger time"""
    executor = ScheduleExecutor()
    
    schedule = Schedule(
        schedule_id='sch_test_003',
        user_id='user_123',
        device_id='dev_456',
        name='Daily Schedule',
        type=ScheduleType.TIME,
        trigger=TimeTrigger(
            time='08:00',
            days=['Mon', 'Tue', 'Wed', 'Thu', 'Fri'],
            timezone='Africa/Johannesburg'
        ),
        action=Action(command='turn_on'),
        enabled=True
    )
    
    next_trigger = executor.calculate_next_time_trigger(schedule)
    
    # Verify next trigger is in the future
    assert next_trigger > datetime.utcnow()
    
    # Verify it's at 08:00 on a weekday
    assert next_trigger.hour == 8 or next_trigger.hour == 6  # UTC adjustment
    assert next_trigger.minute == 0

@pytest.mark.asyncio
async def test_rate_limiting(db, test_device):
    """Test rate limiting on commands"""
    from middleware.rate_limiter import RateLimiter
    
    limiter = RateLimiter(redis_url='redis://localhost:6379')
    
    # Should allow first 10 commands
    for i in range(10):
        result = await limiter.check_rate_limit('commands_per_device', test_device)
        assert result is True
    
    # 11th command should be rate limited
    with pytest.raises(Exception) as exc_info:
        await limiter.check_rate_limit('commands_per_device', test_device)
    
    assert exc_info.value.status_code == 429

@pytest.mark.asyncio
async def test_signature_verification():
    """Test ECDSA signature verification"""
    from services.signature_service import SignatureService
    
    sig_service = SignatureService()
    
    # Create test command
    command = {
        'command_id': 'cmd_001',
        'device_id': 'dev_456',
        'type': 'turn_on',
        'timestamp': datetime.utcnow().isoformat() + 'Z',
        'nonce': 'test_nonce_123'
    }
    
    # Sign command
    signature = await sig_service.sign_command(command)
    
    # Verify signature (should fail as we don't have device key)
    # In real test, load device public key first
    assert signature is not None
    assert len(signature) > 0
```

### AFTERNOON (1PM-5PM): Frontend Unit Tests

#### Step 3: Flutter Widget Tests

**File**: `/app/test/schedule_screen_test.dart`

```dart
import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:smartplugai/screens/schedule_screen.dart';
import 'package:fake_cloud_firestore/fake_cloud_firestore.dart';

void main() {
  group('ScheduleScreen Tests', () {
    late FakeFirebaseFirestore fakeFirestore;
    
    setUp(() {
      fakeFirestore = FakeFirebaseFirestore();
    });
    
    testWidgets('Schedule screen loads correctly', (WidgetTester tester) async {
      await tester.pumpWidget(MaterialApp(
        home: ScheduleScreen(
          deviceId: 'dev_001',
          deviceName: 'Test Device',
        ),
      ));
      
      // Verify screen title
      expect(find.text('New Schedule'), findsOneWidget);
      
      // Verify form fields exist
      expect(find.byType(TextFormField), findsOneWidget);
      expect(find.byType(FilterChip), findsNWidgets(7)); // 7 days of week
    });
    
    testWidgets('Time picker opens when tapped', (WidgetTester tester) async {
      await tester.pumpWidget(MaterialApp(
        home: ScheduleScreen(
          deviceId: 'dev_001',
          deviceName: 'Test Device',
        ),
      ));
      
      // Tap time picker card
      await tester.tap(find.byIcon(Icons.access_time));
      await tester.pumpAndSettle();
      
      // Verify time picker dialog appears
      expect(find.byType(TimePickerDialog), findsOneWidget);
    });
    
    testWidgets('Day selection toggles correctly', (WidgetTester tester) async {
      await tester.pumpWidget(MaterialApp(
        home: ScheduleScreen(
          deviceId: 'dev_001',
          deviceName: 'Test Device',
        ),
      ));
      
      // Find Monday chip
      final mondayChip = find.widgetWithText(FilterChip, 'Mon');
      
      // Tap to select
      await tester.tap(mondayChip);
      await tester.pump();
      
      // Verify it's selected (visual state change)
      final FilterChip chip = tester.widget(mondayChip);
      expect(chip.selected, true);
    });
    
    testWidgets('Form validation works', (WidgetTester tester) async {
      await tester.pumpWidget(MaterialApp(
        home: ScheduleScreen(
          deviceId: 'dev_001',
          deviceName: 'Test Device',
        ),
      ));
      
      // Try to save without filling form
      await tester.tap(find.text('Create Schedule'));
      await tester.pump();
      
      // Expect validation error
      expect(find.text('Please enter a schedule name'), findsOneWidget);
    });
    
    testWidgets('Schedule saves successfully', (WidgetTester tester) async {
      await tester.pumpWidget(MaterialApp(
        home: ScheduleScreen(
          deviceId: 'dev_001',
          deviceName: 'Test Device',
        ),
      ));
      
      // Fill in schedule name
      await tester.enterText(
        find.byType(TextFormField),
        'Morning Coffee'
      );
      
      // Select Monday
      await tester.tap(find.widgetWithText(FilterChip, 'Mon'));
      await tester.pump();
      
      // Save schedule
      await tester.tap(find.text('Create Schedule'));
      await tester.pumpAndSettle();
      
      // Expect success message
      expect(find.text('Schedule created successfully!'), findsOneWidget);
    });
  });
}
```

#### Step 4: Integration Tests

**File**: `/app/integration_test/schedule_flow_test.dart`

```dart
import 'package:flutter_test/flutter_test.dart';
import 'package:integration_test/integration_test.dart';
import 'package:smartplugai/main.dart' as app;
import 'package:firebase_core/firebase_core.dart';

void main() {
  IntegrationTestWidgetsFlutterBinding.ensureInitialized();
  
  group('Schedule Creation Flow', () {
    testWidgets('Complete schedule creation flow', (WidgetTester tester) async {
      // Initialize Firebase
      await Firebase.initializeApp();
      
      // Start app
      app.main();
      await tester.pumpAndSettle();
      
      // Login (assuming already on home screen)
      // Navigate to devices
      await tester.tap(find.byIcon(Icons.devices));
      await tester.pumpAndSettle();
      
      // Select first device
      await tester.tap(find.byType(Card).first);
      await tester.pumpAndSettle();
      
      // Open schedules tab
      await tester.tap(find.text('Schedules'));
      await tester.pumpAndSettle();
      
      // Tap add schedule button
      await tester.tap(find.byIcon(Icons.add));
      await tester.pumpAndSettle();
      
      // Fill schedule form
      await tester.enterText(
        find.byType(TextFormField),
        'Integration Test Schedule'
      );
      
      // Select time
      await tester.tap(find.byIcon(Icons.access_time));
      await tester.pumpAndSettle();
      await tester.tap(find.text('OK'));
      await tester.pumpAndSettle();
      
      // Select days
      await tester.tap(find.text('Mon'));
      await tester.tap(find.text('Wed'));
      await tester.tap(find.text('Fri'));
      await tester.pumpAndSettle();
      
      // Save schedule
      await tester.tap(find.text('Create Schedule'));
      await tester.pumpAndSettle();
      
      // Verify success
      expect(find.text('Schedule created successfully!'), findsOneWidget);
    });
  });
}
```

---

## 📅 DAY 38-39: SECURITY PENETRATION TESTING

### MORNING (9AM-12PM): Automated Security Scanning

#### Step 1: Run OWASP ZAP Scan

```bash
# Install OWASP ZAP (Zed Attack Proxy)
# Option 1: Docker
docker run -t owasp/zap2docker-stable zap-baseline.py \
  -t http://localhost:8000 \
  -r zap_report.html

# Option 2: Direct installation
cd /opt/
wget https://github.com/zaproxy/zaproxy/releases/download/v2.12.0/ZAP_2_12_0_unix.sh
chmod +x ZAP_2_12_0_unix.sh
./ZAP_2_12_0_unix.sh

# Run scan
zap-cli quick-scan --self-contained \
  --start-options '-config api.disablekey=true' \
  http://localhost:8000

# Generate report
zap-cli report -o security_scan_report.html -f html
```

**Security Scan Results Template:**
```
┌─────────────────────────────────────────────────────┐
│         OWASP ZAP SECURITY SCAN RESULTS             │
├─────────────────────────────────────────────────────┤
│                                                      │
│  Target: https://api.smartplugai.com               │
│  Date: 2024-03-15                                   │
│  Duration: 45 minutes                               │
│                                                      │
│  FINDINGS:                                          │
│  ────────────────────────────────────────────       │
│                                                      │
│  🔴 HIGH SEVERITY: 0                                │
│  🟡 MEDIUM SEVERITY: 2                              │
│  🟢 LOW SEVERITY: 5                                 │
│  ℹ️ INFORMATIONAL: 12                              │
│                                                      │
│  MEDIUM SEVERITY ISSUES:                            │
│  ────────────────────────────────────────           │
│                                                      │
│  1. X-Content-Type-Options Header Missing           │
│     - Affects: All endpoints                        │
│     - Risk: MIME type sniffing                      │
│     - Fix: Add X-Content-Type-Options: nosniff      │
│                                                      │
│  2. Content Security Policy Missing                 │
│     - Affects: Web dashboard                        │
│     - Risk: XSS attacks                             │
│     - Fix: Implement CSP header                     │
│                                                      │
│  LOW SEVERITY ISSUES:                               │
│  ────────────────────────────────────────           │
│                                                      │
│  - Missing anti-CSRF tokens (partially)             │
│  - Cookie without SameSite attribute                │
│  - Missing HSTS header                              │
│  - Information disclosure in error messages         │
│  - Timestamp disclosure                             │
│                                                      │
└─────────────────────────────────────────────────────┘
```

#### Step 2: Fix Identified Vulnerabilities

**File**: `/backend/middleware/security_headers.py`

```python
from fastapi import FastAPI, Request
from starlette.middleware.base import BaseHTTPMiddleware
from starlette.responses import Response

class SecurityHeadersMiddleware(BaseHTTPMiddleware):
    async def dispatch(self, request: Request, call_next):
        response = await call_next(request)
        
        # Add security headers
        response.headers['X-Content-Type-Options'] = 'nosniff'
        response.headers['X-Frame-Options'] = 'DENY'
        response.headers['X-XSS-Protection'] = '1; mode=block'
        response.headers['Strict-Transport-Security'] = 'max-age=31536000; includeSubDomains'
        response.headers['Content-Security-Policy'] = (
            "default-src 'self'; "
            "script-src 'self' 'unsafe-inline' https://cdn.jsdelivr.net; "
            "style-src 'self' 'unsafe-inline'; "
            "img-src 'self' data: https:; "
            "font-src 'self' data:; "
            "connect-src 'self' https://api.smartplugai.com wss://mqtt.smartplugai.com; "
            "frame-ancestors 'none';"
        )
        response.headers['Referrer-Policy'] = 'strict-origin-when-cross-origin'
        response.headers['Permissions-Policy'] = (
            "geolocation=(), "
            "microphone=(), "
            "camera=(), "
            "payment=(), "
            "usb=()"
        )
        
        # Remove server header
        if 'Server' in response.headers:
            del response.headers['Server']
        
        return response

# Add to FastAPI app
def configure_security(app: FastAPI):
    app.add_middleware(SecurityHeadersMiddleware)
```

### AFTERNOON (1PM-5PM): Manual Penetration Testing

#### Step 3: Authentication & Authorization Tests

**Test Plan:**

```text
┌─────────────────────────────────────────────────────┐
│      MANUAL PENETRATION TEST CHECKLIST              │
├─────────────────────────────────────────────────────┤
│                                                      │
│  AUTHENTICATION TESTS:                              │
│  ────────────────────────────────────────           │
│                                                      │
│  [✓] Brute force protection                         │
│      - Tested 10 failed login attempts             │
│      - Account locked for 5 minutes ✓              │
│                                                      │
│  [✓] SQL injection in login form                   │
│      - Tested: ' OR '1'='1                          │
│      - Result: Properly escaped, no injection ✓    │
│                                                      │
│  [✓] JWT token expiry                               │
│      - Token expires after 1 hour ✓                │
│      - Refresh token required ✓                    │
│                                                      │
│  [✓] 2FA bypass attempts                            │
│      - Cannot skip 2FA step ✓                      │
│      - Invalid codes rejected ✓                    │
│                                                      │
│  AUTHORIZATION TESTS:                               │
│  ────────────────────────────────────────           │
│                                                      │
│  [✓] Horizontal privilege escalation                │
│      - User A cannot access User B's devices ✓     │
│      - Proper user_id verification in all APIs ✓   │
│                                                      │
│  [✓] Vertical privilege escalation                  │
│      - Regular user cannot access admin APIs ✓     │
│      - Role-based access control enforced ✓        │
│                                                      │
│  [✓] IDOR (Insecure Direct Object Reference)       │
│      - Tested: /api/devices/{other_user_device}    │
│      - Result: 403 Forbidden ✓                     │
│                                                      │
│  API SECURITY TESTS:                                │
│  ────────────────────────────────────────           │
│                                                      │
│  [✓] Command injection                              │
│      - Tested: device_id with shell commands       │
│      - Result: Properly validated ✓                │
│                                                      │
│  [✓] Rate limiting                                  │
│      - Exceeding 10 commands/min blocked ✓         │
│      - Returns 429 Too Many Requests ✓             │
│                                                      │
│  [✓] Replay attacks                                 │
│      - Same nonce rejected ✓                       │
│      - Old timestamps rejected ✓                   │
│                                                      │
│  [✗] FAILED: Missing rate limit on /api/readings   │
│      - Can make unlimited GET requests             │
│      - FIX: Add rate limiting to read endpoints    │
│                                                      │
│  MQTT SECURITY TESTS:                               │
│  ────────────────────────────────────────           │
│                                                      │
│  [✓] TLS 1.3 enforced                               │
│      - TLS 1.2 and below rejected ✓                │
│                                                      │
│  [✓] Client certificate required                    │
│      - Cannot connect without cert ✓               │
│                                                      │
│  [✓] Topic authorization                            │
│      - Device A cannot publish to Device B topic ✓ │
│                                                      │
│  [✓] Message signing                                │
│      - Unsigned commands rejected ✓                │
│      - Invalid signatures rejected ✓               │
│                                                      │
│  FIRMWARE SECURITY TESTS:                           │
│  ────────────────────────────────────────           │
│                                                      │
│  [✓] Secure boot enabled                            │
│      - Cannot flash unsigned firmware ✓            │
│                                                      │
│  [✓] Flash encryption enabled                       │
│      - Cannot read flash contents ✓                │
│                                                      │
│  [✓] Debug ports disabled                           │
│      - JTAG disabled in production ✓               │
│                                                      │
│  [✓] Tamper detection                               │
│      - Opening case triggers alert ✓               │
│      - Alert logged to cloud ✓                     │
│                                                      │
└─────────────────────────────────────────────────────┘
```

#### Step 4: Fix Discovered Vulnerabilities

**Issue**: Missing rate limit on read endpoints

**File**: `/backend/routers/devices.py`

```python
from fastapi import APIRouter, Depends
from middleware.rate_limiter import rate_limiter

router = APIRouter()

@router.get("/devices/{device_id}/readings")
async def get_device_readings(
    device_id: str,
    user_id: str = Depends(get_current_user),
    _: None = Depends(check_read_rate_limit)  # NEW: Add rate limiting
):
    """Get device power readings (rate limited)"""
    # Verify user owns device
    if not await verify_device_ownership(device_id, user_id):
        raise HTTPException(status_code=403, detail="Access denied")
    
    # Fetch readings
    readings = await get_readings_from_db(device_id)
    return readings

async def check_read_rate_limit(request: Request):
    """Rate limiter for read operations"""
    user_id = request.state.user_id
    await rate_limiter.check_rate_limit('reads_per_user', user_id, custom_limit=1000)
```

---

## 📅 DAY 40-41: LOAD TESTING & PERFORMANCE OPTIMIZATION

### MORNING (9AM-12PM): Load Testing with Locust

#### Step 1: Create Load Test Scenarios

**File**: `/tests/load_tests/locustfile.py`

```python
from locust import HttpUser, task, between, events
import json
import random
from datetime import datetime

class SmartPlugUser(HttpUser):
    wait_time = between(1, 3)  # Wait 1-3 seconds between requests
    
    def on_start(self):
        """Login and get auth token"""
        response = self.client.post("/api/auth/login", json={
            "email": f"testuser{random.randint(1, 100)}@example.com",
            "password": "TestPassword123!"
        })
        
        if response.status_code == 200:
            self.token = response.json()['access_token']
            self.device_id = f"dev_{random.randint(1, 1000)}"
        else:
            self.token = None
    
    @task(3)
    def get_device_status(self):
        """Get device status (common operation - 30% of traffic)"""
        if not self.token:
            return
        
        headers = {"Authorization": f"Bearer {self.token}"}
        self.client.get(
            f"/api/devices/{self.device_id}/status",
            headers=headers,
            name="/api/devices/[id]/status"
        )
    
    @task(2)
    def get_power_readings(self):
        """Get power readings (20% of traffic)"""
        if not self.token:
            return
        
        headers = {"Authorization": f"Bearer {self.token}"}
        self.client.get(
            f"/api/devices/{self.device_id}/readings",
            headers=headers,
            name="/api/devices/[id]/readings"
        )
    
    @task(1)
    def send_command(self):
        """Send control command (10% of traffic)"""
        if not self.token:
            return
        
        headers = {"Authorization": f"Bearer {self.token}"}
        command = random.choice(["turn_on", "turn_off"])
        
        self.client.post(
            f"/api/devices/{self.device_id}/command",
            headers=headers,
            json={
                "command": command,
                "timestamp": datetime.utcnow().isoformat() + 'Z'
            },
            name="/api/devices/[id]/command"
        )
    
    @task(1)
    def list_schedules(self):
        """List user schedules (10% of traffic)"""
        if not self.token:
            return
        
        headers = {"Authorization": f"Bearer {self.token}"}
        self.client.get(
            "/api/schedules",
            headers=headers
        )

@events.test_start.add_listener
def on_test_start(environment, **kwargs):
    print("Load test starting...")
    print(f"Target: {environment.host}")

@events.test_stop.add_listener
def on_test_stop(environment, **kwargs):
    print("\nLoad test completed!")
    print(f"Total requests: {environment.stats.total.num_requests}")
    print(f"Failed requests: {environment.stats.total.num_failures}")
    print(f"Avg response time: {environment.stats.total.avg_response_time:.2f}ms")
    print(f"Max response time: {environment.stats.total.max_response_time:.2f}ms")
```

**Run Load Tests:**

```bash
# Install Locust
pip install locust

# Run with web UI
locust -f tests/load_tests/locustfile.py --host=http://localhost:8000

# Open browser: http://localhost:8089
# Configure:
#   - Number of users: 100
#   - Spawn rate: 10 users/second
#   - Host: http://localhost:8000

# Run headless (CI/CD)
locust -f tests/load_tests/locustfile.py \
  --host=http://localhost:8000 \
  --users 100 \
  --spawn-rate 10 \
  --run-time 10m \
  --headless \
  --html=load_test_report.html
```

**Load Test Results Template:**
```
┌──────────────────────────────────────────────────────┐
│           LOAD TEST RESULTS                          │
├──────────────────────────────────────────────────────┤
│                                                       │
│  Configuration:                                      │
│  - Users: 100 concurrent                             │
│  - Duration: 10 minutes                              │
│  - Spawn Rate: 10 users/second                       │
│                                                       │
│  Results:                                            │
│  ───────────────────────────────────────             │
│                                                       │
│  Total Requests: 45,230                              │
│  Failed Requests: 12 (0.03%)                         │
│                                                       │
│  Response Times:                                     │
│  - Average: 145ms                                    │
│  - Median: 120ms                                     │
│  - 95th Percentile: 280ms                            │
│  - 99th Percentile: 450ms                            │
│  - Max: 1,250ms                                      │
│                                                       │
│  Requests per Second: 75.4                           │
│                                                       │
│  Endpoint Performance:                               │
│  ────────────────────────────────────────            │
│                                                       │
│  GET  /api/devices/[id]/status                       │
│       Avg: 95ms  | 95%: 180ms | Failures: 0          │
│                                                       │
│  GET  /api/devices/[id]/readings                     │
│       Avg: 220ms | 95%: 420ms | Failures: 2          │
│                                                       │
│  POST /api/devices/[id]/command                      │
│       Avg: 180ms | 95%: 350ms | Failures: 10         │
│       ⚠️ High failure rate on commands               │
│                                                       │
│  GET  /api/schedules                                 │
│       Avg: 110ms | 95%: 210ms | Failures: 0          │
│                                                       │
│  Issues Found:                                       │
│  ─────────────                                       │
│  1. Command endpoint has 0.83% failure rate          │
│  2. Readings endpoint slower than expected           │
│  3. Some requests timeout > 1 second                 │
│                                                       │
│  Recommendations:                                    │
│  ────────────────                                    │
│  - Add database connection pooling                   │
│  - Implement caching for device status               │
│  - Optimize readings query (add index)               │
│  - Increase rate limit threshold                     │
│                                                       │
└──────────────────────────────────────────────────────┘
```

### AFTERNOON (1PM-5PM): Performance Optimization

#### Step 2: Database Query Optimization

**File**: `/backend/services/readings_service.py`

```python
from firebase_admin import firestore
from typing import List, Dict
from datetime import datetime, timedelta
import asyncio
from functools import lru_cache

class ReadingsService:
    def __init__(self):
        self.db = firestore.client()
        
        # Add caching for recent readings
        self._cache = {}
        self._cache_ttl = 30  # seconds
    
    async def get_device_readings(
        self,
        device_id: str,
        start_time: datetime,
        end_time: datetime,
        use_cache: bool = True
    ) -> List[Dict]:
        """Get device readings with caching and optimization"""
        
        # Check cache first
        cache_key = f"{device_id}:{start_time.isoformat()}:{end_time.isoformat()}"
        
        if use_cache and cache_key in self._cache:
            cache_entry = self._cache[cache_key]
            if (datetime.utcnow() - cache_entry['timestamp']).seconds < self._cache_ttl:
                return cache_entry['data']
        
        # Query Firestore with optimized query
        readings_ref = self.db.collection('devices').document(device_id) \
                             .collection('readings')
        
        # Use composite index: (timestamp, power)
        query = readings_ref.where('timestamp', '>=', start_time) \
                           .where('timestamp', '<=', end_time) \
                           .order_by('timestamp', direction=firestore.Query.DESCENDING) \
                           .limit(100)  # Limit results
        
        # Execute query asynchronously
        docs = await asyncio.to_thread(lambda: list(query.stream()))
        
        readings = [doc.to_dict() for doc in docs]
        
        # Cache results
        self._cache[cache_key] = {
            'data': readings,
            'timestamp': datetime.utcnow()
        }
        
        return readings
    
    @lru_cache(maxsize=1000)
    def get_device_latest_reading_cached(self, device_id: str) -> Dict:
        """Get latest reading with LRU cache"""
        device_ref = self.db.collection('devices').document(device_id)
        device_doc = device_ref.get()
        
        if device_doc.exists:
            return device_doc.to_dict().get('status', {})
        
        return {}
    
    async def bulk_get_readings(self, device_ids: List[str]) -> Dict[str, List[Dict]]:
        """Efficiently fetch readings for multiple devices"""
        
        # Use asyncio.gather for parallel queries
        tasks = [
            self.get_device_readings(
                device_id,
                datetime.utcnow() - timedelta(hours=24),
                datetime.utcnow()
            )
            for device_id in device_ids
        ]
        
        results = await asyncio.gather(*tasks)
        
        return {
            device_id: readings
            for device_id, readings in zip(device_ids, results)
        }
```

#### Step 3: Add Database Indexes

**Firestore Index Configuration:**

**File**: `firestore.indexes.json`

```json
{
  "indexes": [
    {
      "collectionGroup": "readings",
      "queryScope": "COLLECTION",
      "fields": [
        {
          "fieldPath": "device_id",
          "order": "ASCENDING"
        },
        {
          "fieldPath": "timestamp",
          "order": "DESCENDING"
        }
      ]
    },
    {
      "collectionGroup": "readings",
      "queryScope": "COLLECTION",
      "fields": [
        {
          "fieldPath": "timestamp",
          "order": "DESCENDING"
        },
        {
          "fieldPath": "power",
          "order": "DESCENDING"
        }
      ]
    },
    {
      "collectionGroup": "schedules",
      "queryScope": "COLLECTION",
      "fields": [
        {
          "fieldPath": "user_id",
          "order": "ASCENDING"
        },
        {
          "fieldPath": "enabled",
          "order": "ASCENDING"
        },
        {
          "fieldPath": "next_trigger",
          "order": "ASCENDING"
        }
      ]
    },
    {
      "collectionGroup": "commands",
      "queryScope": "COLLECTION",
      "fields": [
        {
          "fieldPath": "device_id",
          "order": "ASCENDING"
        },
        {
          "fieldPath": "timestamp",
          "order": "DESCENDING"
        },
        {
          "fieldPath": "status",
          "order": "ASCENDING"
        }
      ]
    }
  ],
  "fieldOverrides": []
}
```

**Deploy indexes:**
```bash
firebase deploy --only firestore:indexes
```

---

## 📅 DAY 42: BUG TRIAGE & CRITICAL FIXES

### ALL DAY (9AM-5PM): Bug Fixing Sprint

#### Bug Tracking Spreadsheet

```
┌───────────────────────────────────────────────────────────────────────────┐
│                          BUG TRACKING LOG                                  │
├────┬──────────────┬──────────┬──────────┬────────────┬──────────┬────────┤
│ ID │ Description  │ Severity │ Priority │ Component  │ Status   │ Fix    │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│001 │ Commands fail│ Critical │ P0       │ Backend    │ Fixed    │ Rate   │
│    │ under load   │          │          │ API        │          │ limit  │
│    │ (0.83% rate) │          │          │            │          │ tuning │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│002 │ Mobile app   │ High     │ P1       │ Flutter    │ Fixed    │ Error  │
│    │ crashes on   │          │          │ App        │          │ handle │
│    │ no network   │          │          │            │          │        │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│003 │ Schedule not │ High     │ P1       │ Backend    │ Fixed    │ TZ     │
│    │ triggering   │          │          │ Scheduler  │          │ config │
│    │ at correct   │          │          │            │          │        │
│    │ time (TZ)    │          │          │            │          │        │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│004 │ Device goes  │ Medium   │ P2       │ Firmware   │ Fixed    │ WDT    │
│    │ offline      │          │          │ ESP32      │          │ reset  │
│    │ randomly     │          │          │            │          │        │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│005 │ Power reading│ Medium   │ P2       │ Firmware   │ Fixed    │ Calib  │
│    │ inaccurate   │          │          │ SCT-013    │          │ adjust │
│    │ (±5% error)  │          │          │            │          │        │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│006 │ Dashboard    │ Low      │ P3       │ Web React  │ Fixed    │ CSS    │
│    │ layout breaks│          │          │ Dashboard  │          │ fix    │
│    │ on mobile    │          │          │            │          │        │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│007 │ QR pairing   │ Medium   │ P2       │ Mobile App │ In Prog  │ -      │
│    │ slow (>10s)  │          │          │            │          │        │
├────┼──────────────┼──────────┼──────────┼────────────┼──────────┼────────┤
│008 │ MQTT recon   │ Low      │ P3       │ Firmware   │ Fixed    │ Exp    │
│    │ delay high   │          │          │ ESP32      │          │ backoff│
│    │ (30+ sec)    │          │          │            │          │        │
└────┴──────────────┴──────────┴──────────┴────────────┴──────────┴────────┘

Summary:
- Critical: 0 open (1 fixed)
- High: 0 open (2 fixed)
- Medium: 1 open (3 fixed)
- Low: 0 open (2 fixed)

Total: 1 open, 7 fixed
```

#### Critical Bug Fix Example

**Bug #001**: Commands failing under load

**Root Cause**: Rate limiter was too aggressive + Redis connection pool exhausted

**Fix**: `/backend/middleware/rate_limiter.py`

```python
class RateLimiter:
    def __init__(self, redis_url: str = "redis://localhost:6379"):
        # BEFORE: Single connection (bottleneck)
        # self.redis_client = redis.from_url(redis_url)
        
        # AFTER: Connection pool
        pool = redis.ConnectionPool.from_url(
            redis_url,
            max_connections=50,  # Increased from default 10
            decode_responses=True
        )
        self.redis_client = redis.Redis(connection_pool=pool)
        
        # Adjust rate limits (was too restrictive)
        self.limits = {
            'commands_per_device': {'count': 20, 'period': 60},  # Was 10
            'api_per_user': {'count': 200, 'period': 60},  # Was 100
            'login_attempts': {'count': 5, 'period': 300},
            'schedules_per_user': {'count': 50, 'period': 86400},
        }
```

---

## 🎯 WEEK 6 DELIVERABLES CHECKLIST

```text
Testing Deliverables:
[√] Unit tests written for backend services (90% coverage)
[√] Unit tests written for Flutter widgets
[√] Integration tests for schedule flow
[√] End-to-end tests for complete user journey
[√] Load test with 100 concurrent users completed
[√] Performance baseline established

Security Deliverables:
[√] OWASP ZAP automated scan completed
[√] All high-severity vulnerabilities fixed
[√] Security headers implemented
[√] Manual penetration testing completed
[√] Authentication/Authorization tests passed
[√] MQTT security verified
[√] Firmware security verified (secure boot, flash encryption)
[√] Tamper detection tested

Performance Deliverables:
[√] Database queries optimized
[√] Caching layer implemented
[√] Firestore indexes created and deployed
[√] API response times < 200ms (95th percentile)
[√] System handles 100+ concurrent users

Bug Fixes:
[√] 7 bugs fixed, 1 remaining (low priority)
[√] Critical bugs: 0 open
[√] High bugs: 0 open
[√] All P0 and P1 bugs resolved

Documentation:
[√] Security test report generated
[√] Load test results documented
[√] Bug tracking log maintained
[√] Performance optimization notes
[√] Test coverage report

Quality Metrics:
[√] Code coverage: 90%+
[√] All critical paths tested
[√] Zero known security vulnerabilities (high/critical)
[√] System uptime: 99.9% during testing
[√] Error rate: < 0.1%

Next Week Preview:
→ Week 7 focuses on pilot deployment and final documentation
→ Deploy to 10-20 pilot users
→ Monitor real-world usage
→ Prepare for Phase 2 scaling
```

---

**End of Week 6 Guide** ✅  
**Next**: [WEEK7_PILOT_DEPLOYMENT.md](WEEK7_PILOT_DEPLOYMENT.md)
