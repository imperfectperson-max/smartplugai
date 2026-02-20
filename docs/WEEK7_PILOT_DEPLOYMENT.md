# WEEK 7: PILOT DEPLOYMENT & DOCUMENTATION - DEEP DIVE

> 📍 **Roadmap Context**: This guide covers pilot deployment for the **Phase 2: Full System** stage (Months 13–18) of the [24-month Smart Plug AI roadmap](ROADMAP.md). See [WEEK7_PILOT_DEPLOYMENT.md](WEEK7_PILOT_DEPLOYMENT.md) for monitoring setup.

📅 **Timeline**: Days 43-49  
🎯 **Goal**: Deploy to pilot users, establish 24/7 monitoring, complete all documentation, and validate system readiness for Phase 2 scaling

## 📅 DAY 43-44: PRODUCTION DEPLOYMENT SETUP

### MORNING (9AM-12PM): Cloud Infrastructure Configuration

#### Step 1: Firebase Production Setup

**Firebase Console Configuration:**

```text
┌────────────────────────────────────────────────────┐
│       FIREBASE PRODUCTION CONFIGURATION            │
├────────────────────────────────────────────────────┤
│                                                     │
│  Project: smartplugai-prod                        │
│                                                     │
│  1. AUTHENTICATION:                                │
│     ✓ Email/Password enabled                      │
│     ✓ Google OAuth enabled                        │
│     ✓ 2FA enforcement for admin accounts          │
│     ✓ Password policy: Min 12 chars, mixed case   │
│                                                     │
│  2. FIRESTORE DATABASE:                            │
│     ✓ Production mode enabled                     │
│     ✓ Security rules deployed                     │
│     ✓ Composite indexes created                   │
│     ✓ Backup schedule: Daily at 2 AM UTC          │
│     ✓ Point-in-time recovery: 7 days              │
│                                                     │
│  3. CLOUD FUNCTIONS:                               │
│     ✓ Node.js 18 runtime                          │
│     ✓ Memory: 512 MB (schedule executor)          │
│     ✓ Timeout: 540 seconds                        │
│     ✓ Environment variables configured            │
│     ✓ Service account with least privilege        │
│                                                     │
│  4. HOSTING (Web Dashboard):                       │
│     ✓ Custom domain: app.smartplugai.com          │
│     ✓ SSL certificate: Auto-provisioned           │
│     ✓ CDN enabled globally                        │
│     ✓ Cache policy: Static 1 year, Dynamic 5 min  │
│                                                     │
│  5. STORAGE (Firmware updates):                    │
│     ✓ Bucket: smartplugai-firmware                │
│     ✓ Versioning enabled                          │
│     ✓ Lifecycle: Delete after 90 days             │
│     ✓ Access: Signed URLs only                    │
│                                                     │
└────────────────────────────────────────────────────┘
```

**Firestore Security Rules (Production):**

**File**: `firestore.rules`

```javascript
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    
    // Helper functions
    function isAuthenticated() {
      return request.auth != null;
    }
    
    function isOwner(userId) {
      return isAuthenticated() && request.auth.uid == userId;
    }
    
    function isAdmin() {
      return isAuthenticated() && 
             get(/databases/$(database)/documents/admins/$(request.auth.uid)).data.role == 'admin';
    }
    
    // Users collection
    match /users/{userId} {
      allow read: if isOwner(userId) || isAdmin();
      allow create: if isOwner(userId);
      allow update: if isOwner(userId);
      allow delete: if isAdmin();
      
      // Sub-collection: user preferences (private)
      match /preferences/{document=**} {
        allow read, write: if isOwner(userId);
      }
    }
    
    // Devices collection
    match /devices/{deviceId} {
      allow read: if isAuthenticated() && 
                     resource.data.owner_id == request.auth.uid;
      allow create: if isAuthenticated() && 
                       request.resource.data.owner_id == request.auth.uid;
      allow update: if isAuthenticated() && 
                       resource.data.owner_id == request.auth.uid &&
                       request.resource.data.owner_id == request.auth.uid;
      allow delete: if isAuthenticated() && 
                       resource.data.owner_id == request.auth.uid;
      
      // Sub-collection: device readings
      match /readings/{readingId} {
        allow read: if isAuthenticated() && 
                       get(/databases/$(database)/documents/devices/$(deviceId)).data.owner_id == request.auth.uid;
        allow create: if true;  // Devices can write readings
        allow update, delete: if false;  // Readings are immutable
      }
      
      // Sub-collection: device commands
      match /commands/{commandId} {
        allow read: if isAuthenticated() && 
                       get(/databases/$(database)/documents/devices/$(deviceId)).data.owner_id == request.auth.uid;
        allow create: if isAuthenticated() && 
                         get(/databases/$(database)/documents/devices/$(deviceId)).data.owner_id == request.auth.uid &&
                         request.resource.data.signature != null;  // Must be signed
        allow update: if true;  // System can update command status
        allow delete: if false;
      }
    }
    
    // Schedules collection
    match /schedules/{scheduleId} {
      allow read: if isAuthenticated() && 
                     resource.data.user_id == request.auth.uid;
      allow create: if isAuthenticated() && 
                       request.resource.data.user_id == request.auth.uid &&
                       request.resource.data.device_id != null;
      allow update: if isAuthenticated() && 
                       resource.data.user_id == request.auth.uid;
      allow delete: if isAuthenticated() && 
                       resource.data.user_id == request.auth.uid;
    }
    
    // Alerts collection
    match /alerts/{alertId} {
      allow read: if isAuthenticated() && 
                     resource.data.user_id == request.auth.uid;
      allow create: if true;  // System can create alerts
      allow update: if isAuthenticated() && 
                       resource.data.user_id == request.auth.uid;  // User can acknowledge
      allow delete: if isAuthenticated() && 
                       resource.data.user_id == request.auth.uid;
    }
    
    // Audit logs (admin only)
    match /audit_logs/{logId} {
      allow read: if isAdmin();
      allow write: if false;  // System writes only
    }
    
    // Default: deny all
    match /{document=**} {
      allow read, write: if false;
    }
  }
}
```

**Deploy to Production:**

```bash
# Deploy security rules
firebase deploy --only firestore:rules --project smartplugai-prod

# Deploy indexes
firebase deploy --only firestore:indexes --project smartplugai-prod

# Deploy Cloud Functions
firebase deploy --only functions --project smartplugai-prod

# Deploy Web Dashboard
cd web/
npm run build
firebase deploy --only hosting --project smartplugai-prod
```

### AFTERNOON (1PM-5PM): MQTT Broker Production Setup

#### Step 2: EMQX MQTT Broker Configuration

**Docker Compose for Production MQTT:**

**File**: `/infrastructure/mqtt/docker-compose.prod.yml`

```yaml
version: '3.8'

services:
  emqx:
    image: emqx/emqx:5.0.0
    container_name: smartplugai-mqtt
    restart: always
    ports:
      - "1883:1883"    # MQTT (disabled in prod)
      - "8883:8883"    # MQTT over TLS
      - "18083:18083"  # Dashboard
    environment:
      - EMQX_NAME=smartplugai
      - EMQX_HOST=mqtt.smartplugai.com
      
      # Authentication
      - EMQX_AUTH__METHOD=mysql
      - EMQX_AUTH__MYSQL__SERVER=mysql:3306
      - EMQX_AUTH__MYSQL__USERNAME=emqx
      - EMQX_AUTH__MYSQL__PASSWORD=${MYSQL_PASSWORD}
      - EMQX_AUTH__MYSQL__DATABASE=emqx_auth
      
      # TLS Configuration
      - EMQX_LISTENER__SSL__EXTERNAL__KEYFILE=/etc/emqx/certs/server.key
      - EMQX_LISTENER__SSL__EXTERNAL__CERTFILE=/etc/emqx/certs/server.crt
      - EMQX_LISTENER__SSL__EXTERNAL__CACERTFILE=/etc/emqx/certs/ca.crt
      - EMQX_LISTENER__SSL__EXTERNAL__VERIFY=verify_peer
      - EMQX_LISTENER__SSL__EXTERNAL__FAIL_IF_NO_PEER_CERT=true
      
      # Performance
      - EMQX_MAX_CONNECTIONS=100000
      - EMQX_LISTENER__TCP__EXTERNAL__MAX_CONNECTIONS=10000
      - EMQX_LISTENER__SSL__EXTERNAL__MAX_CONNECTIONS=10000
      
      # Security
      - EMQX_ALLOW_ANONYMOUS=false
      - EMQX_ACL_NOMATCH=deny
      
    volumes:
      - ./certs:/etc/emqx/certs:ro
      - ./emqx_data:/opt/emqx/data
      - ./emqx_log:/opt/emqx/log
      - ./acl.conf:/opt/emqx/etc/acl.conf:ro
    networks:
      - smartplug_network
    depends_on:
      - mysql
  
  mysql:
    image: mysql:8.0
    container_name: smartplugai-mysql
    restart: always
    environment:
      - MYSQL_ROOT_PASSWORD=${MYSQL_ROOT_PASSWORD}
      - MYSQL_DATABASE=emqx_auth
      - MYSQL_USER=emqx
      - MYSQL_PASSWORD=${MYSQL_PASSWORD}
    volumes:
      - mysql_data:/var/lib/mysql
      - ./init_auth.sql:/docker-entrypoint-initdb.d/init.sql:ro
    networks:
      - smartplug_network
  
  # Monitoring
  prometheus:
    image: prom/prometheus:latest
    container_name: smartplugai-prometheus
    restart: always
    ports:
      - "9090:9090"
    volumes:
      - ./prometheus.yml:/etc/prometheus/prometheus.yml:ro
      - prometheus_data:/prometheus
    command:
      - '--config.file=/etc/prometheus/prometheus.yml'
      - '--storage.tsdb.path=/prometheus'
    networks:
      - smartplug_network
  
  grafana:
    image: grafana/grafana:latest
    container_name: smartplugai-grafana
    restart: always
    ports:
      - "3000:3000"
    environment:
      - GF_SECURITY_ADMIN_PASSWORD=${GRAFANA_PASSWORD}
      - GF_INSTALL_PLUGINS=grafana-clock-panel
    volumes:
      - grafana_data:/var/lib/grafana
      - ./grafana/dashboards:/etc/grafana/provisioning/dashboards
    networks:
      - smartplug_network
    depends_on:
      - prometheus

networks:
  smartplug_network:
    driver: bridge

volumes:
  mysql_data:
  prometheus_data:
  grafana_data:
```

**MQTT ACL Configuration:**

**File**: `/infrastructure/mqtt/acl.conf`

```conf
%% ACL for Smart Plug AI MQTT

%% Allow devices to publish to their own topics
{allow, {user, "device-{device_id}"}, publish, ["smartplug/devices/+/telemetry"]}.
{allow, {user, "device-{device_id}"}, publish, ["smartplug/devices/+/status"]}.
{allow, {user, "device-{device_id}"}, publish, ["smartplug/devices/+/alerts"]}.

%% Allow devices to subscribe to their own command topics
{allow, {user, "device-{device_id}"}, subscribe, ["smartplug/devices/+/commands"]}.

%% Allow backend to publish commands
{allow, {user, "backend-service"}, publish, ["smartplug/devices/+/commands"]}.

%% Allow backend to subscribe to all device topics
{allow, {user, "backend-service"}, subscribe, ["smartplug/devices/#"]}.
{allow, {user, "backend-service"}, subscribe, ["smartplug/alerts/#"]}.

%% Deny everything else
{deny, all}.
```

**Start MQTT Broker:**

```bash
cd infrastructure/mqtt/

# Set environment variables
export MYSQL_ROOT_PASSWORD="<strong-password>"
export MYSQL_PASSWORD="<emqx-password>"
export GRAFANA_PASSWORD="<grafana-password>"

# Start services
docker-compose -f docker-compose.prod.yml up -d

# Verify EMQX is running
docker logs smartplugai-mqtt

# Access EMQX Dashboard
# URL: https://mqtt.smartplugai.com:18083
# Default: admin / public (CHANGE THIS!)
```

---

## 📅 DAY 45: PILOT USER ONBOARDING

### MORNING (9AM-12PM): Device Provisioning

#### Step 1: Bulk Device Provisioning Script

**File**: `/scripts/provision_pilot_devices.py`

```python
#!/usr/bin/env python3

import serial
import time
import json
from firebase_admin import firestore, initialize_app, credentials
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend

# Initialize Firebase
cred = credentials.Certificate('serviceAccountKey.json')
initialize_app(cred)
db = firestore.client()

class DeviceProvisioner:
    def __init__(self, serial_port='/dev/ttyUSB0', baud_rate=115200):
        self.serial = serial.Serial(serial_port, baud_rate, timeout=5)
        time.sleep(2)  # Wait for ESP32 to reset
    
    def provision_device(self, device_number):
        """Provision a single device"""
        print(f"\n{'='*60}")
        print(f"Provisioning Device #{device_number}")
        print(f"{'='*60}")
        
        # Generate device ID
        device_id = f"SP001-{device_number:04d}"
        print(f"Device ID: {device_id}")
        
        # Step 1: Get ATECC608A serial number
        print("\n[1/7] Reading ATECC608A serial number...")
        self.serial.write(b"get_serial\n")
        atecc_serial = self.serial.readline().decode().strip()
        print(f"  ✓ ATECC608A Serial: {atecc_serial}")
        
        # Step 2: Generate key pair in ATECC608A
        print("\n[2/7] Generating ECC P-256 key pair...")
        self.serial.write(b"gen_keypair\n")
        response = self.serial.readline().decode().strip()
        if response != "OK":
            print(f"  ✗ Failed: {response}")
            return False
        print("  ✓ Key pair generated")
        
        # Step 3: Get public key
        print("\n[3/7] Reading device public key...")
        self.serial.write(b"get_pubkey\n")
        pubkey_hex = self.serial.readline().decode().strip()
        print(f"  ✓ Public key: {pubkey_hex[:32]}...")
        
        # Step 4: Lock ATECC608A configuration
        print("\n[4/7] Locking ATECC608A configuration...")
        self.serial.write(b"lock_config\n")
        response = self.serial.readline().decode().strip()
        if "LOCKED" in response:
            print("  ✓ Configuration locked (irreversible)")
        else:
            print(f"  ⚠️ {response}")
        
        # Step 5: Set device ID in EEPROM
        print("\n[5/7] Writing device ID to EEPROM...")
        self.serial.write(f"set_device_id {device_id}\n".encode())
        response = self.serial.readline().decode().strip()
        print(f"  ✓ Device ID stored: {device_id}")
        
        # Step 6: Enable secure boot and flash encryption
        print("\n[6/7] Enabling secure boot and flash encryption...")
        print("  ⚠️ This step requires manual efuse burning:")
        print("     esptool.py --port /dev/ttyUSB0 burn_efuse FLASH_CRYPT_CNT")
        print("     esptool.py --port /dev/ttyUSB0 burn_efuse ABS_DONE_0")
        print("  ⏸️ Press Enter when done...")
        input()
        
        # Step 7: Register device in Firestore
        print("\n[7/7] Registering device in cloud...")
        device_doc = {
            'device_id': device_id,
            'serial_number': device_id,
            'atecc_serial': atecc_serial,
            'public_key': pubkey_hex,
            'model': 'SP-001',
            'firmware_version': '1.0.0',
            'provisioned_at': firestore.SERVER_TIMESTAMP,
            'status': {
                'online': False,
                'power': 0,
                'current': 0,
                'voltage': 0
            },
            'owner_id': None,  # To be assigned during pairing
            'secure_boot_enabled': True,
            'flash_encryption_enabled': True
        }
        
        db.collection('devices').document(device_id).set(device_doc)
        print(f"  ✓ Device registered in Firestore")
        
        # Generate QR code for pairing
        pairing_data = {
            'device_id': device_id,
            'public_key': pubkey_hex,
            'pairing_url': f'https://app.smartplugai.com/pair/{device_id}'
        }
        
        print("\n" + "="*60)
        print("✅ PROVISIONING COMPLETE!")
        print("="*60)
        print(f"\nDevice ID: {device_id}")
        print(f"Pairing URL: {pairing_data['pairing_url']}")
        print("\nNext steps:")
        print("1. Generate QR code sticker with pairing URL")
        print("2. Apply QR code to device enclosure")
        print("3. Apply tamper-evident seals")
        print("4. Assign to pilot user")
        
        return True
    
    def close(self):
        self.serial.close()

def main():
    """Provision multiple devices"""
    print("Smart Plug AI - Device Provisioning Tool")
    print("=========================================\n")
    
    num_devices = int(input("Number of devices to provision: "))
    start_number = int(input("Starting device number: "))
    
    provisioner = DeviceProvisioner()
    
    successful = 0
    failed = 0
    
    for i in range(num_devices):
        device_num = start_number + i
        
        input(f"\n📋 Connect device #{device_num} and press Enter...")
        
        if provisioner.provision_device(device_num):
            successful += 1
            print("\n✅ SUCCESS - Device ready for deployment")
        else:
            failed += 1
            print("\n❌ FAILED - Check logs and retry")
        
        print("\n" + "─"*60)
    
    provisioner.close()
    
    print(f"\n{'='*60}")
    print("PROVISIONING SUMMARY")
    print(f"{'='*60}")
    print(f"Total devices: {num_devices}")
    print(f"Successful: {successful}")
    print(f"Failed: {failed}")
    print(f"{'='*60}\n")

if __name__ == "__main__":
    main()
```

**Run Provisioning:**

```bash
# Install dependencies
pip install pyserial firebase-admin cryptography

# Run provisioning script
python3 scripts/provision_pilot_devices.py

# Example output:
# Number of devices to provision: 20
# Starting device number: 1
# 
# Connect device #1 and press Enter...
# [Provisioning process...]
# ✅ SUCCESS - Device ready for deployment
```

### AFTERNOON (1PM-5PM): User Onboarding

#### Step 2: Pilot User Welcome Kit

**Pilot User Welcome Email:**

```html
<!DOCTYPE html>
<html>
<head>
    <style>
        body { font-family: Arial, sans-serif; line-height: 1.6; color: #333; }
        .container { max-width: 600px; margin: 0 auto; padding: 20px; }
        .header { background: #007bff; color: white; padding: 20px; text-align: center; }
        .content { padding: 20px; background: #f9f9f9; }
        .step { margin: 20px 0; padding: 15px; background: white; border-left: 4px solid #007bff; }
        .button { display: inline-block; padding: 12px 24px; background: #007bff; 
                  color: white; text-decoration: none; border-radius: 5px; }
        .warning { background: #fff3cd; padding: 15px; border-left: 4px solid #ffc107; margin: 15px 0; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🎉 Welcome to Smart Plug AI Pilot Program!</h1>
        </div>
        
        <div class="content">
            <p>Dear Pilot User,</p>
            
            <p>Thank you for joining our pilot program! You're about to experience South Africa's 
            most secure smart plug system with bank-grade security and intelligent automation.</p>
            
            <h2>📦 What's in Your Package:</h2>
            <ul>
                <li>1x Smart Plug AI device (Model: SP-001)</li>
                <li>1x Quick Start Guide</li>
                <li>1x Safety Information Card</li>
                <li>1x QR Pairing Sticker (on device)</li>
            </ul>
            
            <h2>🚀 Quick Setup (5 minutes):</h2>
            
            <div class="step">
                <h3>Step 1: Download the App</h3>
                <p>Get the Smart Plug AI app:</p>
                <a href="https://play.google.com/store" class="button">Android</a>
                <a href="https://apps.apple.com" class="button">iOS</a>
            </div>
            
            <div class="step">
                <h3>Step 2: Create Your Account</h3>
                <p>Open the app and sign up with:</p>
                <ul>
                    <li>Your email address</li>
                    <li>A strong password (min. 12 characters)</li>
                    <li>Enable 2-Factor Authentication (recommended)</li>
                </ul>
            </div>
            
            <div class="step">
                <h3>Step 3: Pair Your Device</h3>
                <p>1. Plug the Smart Plug into a wall socket<br>
                   2. In the app, tap "Add Device"<br>
                   3. Scan the QR code on your device<br>
                   4. Follow the WiFi setup wizard<br>
                   5. Done! Your device is now online</p>
            </div>
            
            <div class="warning">
                <h3>⚠️ Safety First:</h3>
                <ul>
                    <li><strong>Max Load:</strong> 2300W (10A at 230V)</li>
                    <li><strong>Indoor use only</strong> - Not water resistant</li>
                    <li><strong>Do not use with:</strong> Space heaters, electric blankets, medical devices</li>
                    <li><strong>Tamper detection:</strong> Device logs if case is opened</li>
                </ul>
            </div>
            
            <h2>💡 Cool Features to Try:</h2>
            <ul>
                <li><strong>Schedules:</strong> Turn devices on/off automatically</li>
                <li><strong>Energy Monitoring:</strong> See real-time power consumption</li>
                <li><strong>Load-Shedding Protection:</strong> Auto-off during outages</li>
                <li><strong>Alerts:</strong> Get notified of unusual activity</li>
            </ul>
            
            <h2>📞 Need Help?</h2>
            <p>We're here to support you:</p>
            <ul>
                <li><strong>Email:</strong> pilot@smartplugai.com</li>
                <li><strong>WhatsApp:</strong> +27 XX XXX XXXX</li>
                <li><strong>Help Center:</strong> https://help.smartplugai.com</li>
            </ul>
            
            <h2>🎁 Special Pilot Program Benefits:</h2>
            <ul>
                <li>Free device and service for 3 months</li>
                <li>Priority support (< 1 hour response time)</li>
                <li>Influence future features with your feedback</li>
                <li>Exclusive discount on future purchases</li>
            </ul>
            
            <p>Thank you for being an early adopter! Your feedback is invaluable in making 
            Smart Plug AI the best it can be.</p>
            
            <p>Best regards,<br>
            <strong>The Smart Plug AI Team</strong></p>
            
            <hr>
            
            <p style="font-size: 0.9em; color: #666;">
            Device Serial: <strong>SP001-XXXX</strong><br>
            Provisioned: March 15, 2024<br>
            Firmware Version: 1.0.0
            </p>
        </div>
    </div>
</body>
</html>
```

---

## 📅 DAY 46-47: 24/7 MONITORING SETUP

### MORNING (9AM-12PM): Monitoring Dashboard Configuration

#### Step 1: Grafana Dashboard Setup

**Grafana Dashboard JSON:**

**File**: `/infrastructure/grafana/dashboards/smartplug_overview.json`

```json
{
  "dashboard": {
    "title": "Smart Plug AI - Production Overview",
    "panels": [
      {
        "id": 1,
        "title": "Active Devices",
        "type": "stat",
        "targets": [
          {
            "expr": "count(smartplug_device_online{status=\"true\"})"
          }
        ],
        "gridPos": {"x": 0, "y": 0, "w": 6, "h": 4}
      },
      {
        "id": 2,
        "title": "Total Power Consumption (kW)",
        "type": "graph",
        "targets": [
          {
            "expr": "sum(smartplug_device_power_watts) / 1000"
          }
        ],
        "gridPos": {"x": 6, "y": 0, "w": 12, "h": 8}
      },
      {
        "id": 3,
        "title": "Command Success Rate (%)",
        "type": "stat",
        "targets": [
          {
            "expr": "(sum(smartplug_commands_success) / sum(smartplug_commands_total)) * 100"
          }
        ],
        "gridPos": {"x": 18, "y": 0, "w": 6, "h": 4}
      },
      {
        "id": 4,
        "title": "MQTT Connections",
        "type": "stat",
        "targets": [
          {
            "expr": "emqx_connections_count"
          }
        ],
        "gridPos": {"x": 0, "y": 4, "w": 6, "h": 4}
      },
      {
        "id": 5,
        "title": "API Response Time (ms)",
        "type": "graph",
        "targets": [
          {
            "expr": "histogram_quantile(0.95, sum(rate(http_request_duration_ms_bucket[5m])) by (le))"
          }
        ],
        "gridPos": {"x": 0, "y": 8, "w": 12, "h": 8}
      },
      {
        "id": 6,
        "title": "Security Alerts (Last 24h)",
        "type": "table",
        "targets": [
          {
            "expr": "smartplug_security_alerts"
          }
        ],
        "gridPos": {"x": 12, "y": 8, "w": 12, "h": 8}
      }
    ],
    "refresh": "30s",
    "time": {
      "from": "now-6h",
      "to": "now"
    }
  }
}
```

### AFTERNOON (1PM-5PM): Alerting & Incident Response

#### Step 2: PagerDuty Integration

**File**: `/backend/services/alerting_service.py`

```python
import httpx
from typing import Dict, List
from enum import Enum

class AlertSeverity(Enum):
    INFO = "info"
    WARNING = "warning"
    ERROR = "error"
    CRITICAL = "critical"

class AlertingService:
    def __init__(self):
        self.pagerduty_api_key = "YOUR_PAGERDUTY_API_KEY"
        self.pagerduty_service_id = "YOUR_SERVICE_ID"
        
        # Alert thresholds
        self.thresholds = {
            'device_offline_minutes': 15,
            'error_rate_percent': 5,
            'response_time_ms': 500,
            'security_alerts_per_hour': 10
        }
    
    async def send_alert(
        self,
        title: str,
        description: str,
        severity: AlertSeverity,
        source: str,
        custom_details: Dict = None
    ):
        """Send alert to PagerDuty"""
        
        # Map severity to PagerDuty
        pd_severity = {
            AlertSeverity.INFO: "info",
            AlertSeverity.WARNING: "warning",
            AlertSeverity.ERROR: "error",
            AlertSeverity.CRITICAL: "critical"
        }[severity]
        
        payload = {
            "routing_key": self.pagerduty_api_key,
            "event_action": "trigger",
            "payload": {
                "summary": title,
                "severity": pd_severity,
                "source": source,
                "custom_details": custom_details or {}
            }
        }
        
        async with httpx.AsyncClient() as client:
            response = await client.post(
                "https://events.pagerduty.com/v2/enqueue",
                json=payload
            )
            
            if response.status_code == 202:
                print(f"✓ Alert sent: {title}")
            else:
                print(f"✗ Failed to send alert: {response.text}")
    
    async def check_system_health(self):
        """Periodic health check"""
        
        # Check 1: Device connectivity
        offline_devices = await self.get_offline_devices()
        if len(offline_devices) > 0:
            await self.send_alert(
                title=f"{len(offline_devices)} devices offline",
                description=f"Devices: {', '.join(offline_devices[:5])}",
                severity=AlertSeverity.WARNING,
                source="health_check",
                custom_details={"offline_count": len(offline_devices)}
            )
        
        # Check 2: Error rate
        error_rate = await self.get_error_rate()
        if error_rate > self.thresholds['error_rate_percent']:
            await self.send_alert(
                title=f"High error rate: {error_rate}%",
                description="Error rate exceeds threshold",
                severity=AlertSeverity.ERROR,
                source="health_check",
                custom_details={"error_rate": error_rate}
            )
        
        # Check 3: Response time
        response_time = await self.get_avg_response_time()
        if response_time > self.thresholds['response_time_ms']:
            await self.send_alert(
                title=f"Slow response time: {response_time}ms",
                description="API response time exceeds threshold",
                severity=AlertSeverity.WARNING,
                source="health_check",
                custom_details={"response_time_ms": response_time}
            )
        
        # Check 4: Security alerts
        security_alerts = await self.get_recent_security_alerts()
        if len(security_alerts) > self.thresholds['security_alerts_per_hour']:
            await self.send_alert(
                title=f"High number of security alerts: {len(security_alerts)}",
                description="Possible security incident",
                severity=AlertSeverity.CRITICAL,
                source="security_monitor",
                custom_details={"alert_count": len(security_alerts)}
            )

# Background task for monitoring
import asyncio

async def monitoring_task():
    """Run continuous monitoring"""
    alerting = AlertingService()
    
    while True:
        try:
            await alerting.check_system_health()
            await asyncio.sleep(300)  # Check every 5 minutes
        except Exception as e:
            print(f"Monitoring error: {e}")
            await asyncio.sleep(60)
```

---

## 📅 DAY 48-49: FINAL DOCUMENTATION

### ALL DAY: Documentation Sprint

#### Deliverable 1: User Manual

**File**: `/docs/USER_MANUAL.md` (Executive Summary)

```markdown
# Smart Plug AI - User Manual
Version 1.0 | March 2024

## Table of Contents
1. Introduction
2. Safety Information
3. Getting Started
4. Features Guide
5. Troubleshooting
6. Technical Specifications
7. Warranty & Support

## 1. Introduction

Welcome to Smart Plug AI, South Africa's most secure smart plug system. 
This manual will guide you through setup, features, and best practices.

## 2. Safety Information

⚠️ **IMPORTANT - READ BEFORE USE**

- **Maximum Load:** 2300W (10A at 230V AC)
- **Indoor Use Only:** Not water resistant
- **Do NOT Use With:**
  - Space heaters
  - Electric blankets
  - Medical devices
  - High-inrush devices (compressors)
- **Tamper Detection:** Opening the case triggers a security alert

## 3. Getting Started

### 3.1 Unboxing
Your package includes:
- 1x Smart Plug AI device
- 1x Quick Start Guide
- 1x Safety Card

### 3.2 App Installation
Download from:
- **Android:** Google Play Store
- **iOS:** Apple App Store
- Search: "Smart Plug AI"

### 3.3 Account Creation
1. Open app
2. Tap "Sign Up"
3. Enter email and create password (min. 12 characters)
4. Verify email address
5. Enable 2-Factor Authentication (recommended)

### 3.4 Device Pairing
1. Plug Smart Plug into wall socket
2. Wait for blue LED to blink (pairing mode)
3. In app, tap "Add Device"
4. Scan QR code on device
5. Connect to WiFi
6. Name your device
7. Done!

## 4. Features Guide

### 4.1 Real-Time Monitoring
- **Power:** Current wattage consumption
- **Energy:** Cumulative kWh usage
- **Cost:** Estimated electricity cost
- **History:** View past consumption

### 4.2 Schedules
Create automated ON/OFF schedules:
1. Tap device → "Schedules"
2. Tap "+" to add
3. Set time and days
4. Choose action (ON/OFF)
5. Optional: Auto-off timer
6. Save

### 4.3 Load-Shedding Protection
Automatically turns off devices during load-shedding:
1. Go to Settings → "Load-Shedding"
2. Select your area
3. Enable "Auto-off during load-shedding"
4. Choose devices to protect

### 4.4 Alerts
Get notified of:
- Unusual power consumption
- Device offline
- Tamper detection
- Schedule failures

## 5. Troubleshooting

### Device Won't Connect to WiFi
- Check WiFi password
- Ensure 2.4GHz WiFi (not 5GHz)
- Move router closer
- Restart device (unplug/plug)

### Device Shows Offline
- Check WiFi connection
- Check power supply
- Check MQTT broker status
- Contact support if persists > 15 min

### Schedule Not Triggering
- Verify time zone settings
- Check schedule is enabled
- Ensure device is online
- Check conditions are met

## 6. Technical Specifications

- **Model:** SP-001
- **Input:** 230V AC, 50Hz
- **Max Load:** 2300W (10A)
- **WiFi:** 2.4GHz 802.11 b/g/n
- **Security:** TLS 1.3, ECDSA signatures
- **Current Sensor:** SCT-013-030 (30A)
- **Accuracy:** ±1%
- **Dimensions:** 110mm x 90mm x 30mm
- **Weight:** 150g
- **Operating Temp:** 0°C to 50°C
- **Certifications:** SABS (pending), ICASA (pending)

## 7. Warranty & Support

### Warranty
- **Period:** 12 months from purchase
- **Coverage:** Manufacturing defects
- **Not Covered:** Misuse, tampering, water damage

### Support
- **Email:** support@smartplugai.com
- **Response Time:** < 24 hours
- **WhatsApp:** +27 XX XXX XXXX
- **Help Center:** https://help.smartplugai.com

---

Smart Plug AI - Secure. Intelligent. Reliable.
```

#### Deliverable 2: Security Audit Report

**File**: `/docs/SECURITY_AUDIT_PHASE1.md` (Summary)

```markdown
# Security Audit Report - Phase 1
Smart Plug AI | March 2024

## Executive Summary

This report documents the security audit conducted for Smart Plug AI Phase 1 
MVP deployment. The system passed all critical security tests and is ready 
for pilot deployment.

## Audit Scope
- Hardware (ESP32-S3, ATECC608A)
- Firmware (secure boot, flash encryption)
- Backend API (FastAPI, Firebase)
- Mobile App (Flutter)
- MQTT Communication (TLS 1.3)
- Cloud Infrastructure

## Security Assessment Results

### ✅ PASSED (Grade: A-)

**Overall Security Score: 92/100**

### Key Strengths:
1. ✓ Secure boot enabled (ESP32-S3)
2. ✓ Flash encryption enabled
3. ✓ ATECC608A secure element for key storage
4. ✓ TLS 1.3 with client certificates (MQTT)
5. ✓ Command signing with ECDSA
6. ✓ Replay attack prevention (nonce + timestamp)
7. ✓ Rate limiting enforced
8. ✓ Tamper detection active
9. ✓ 2FA available for users
10. ✓ Firestore security rules enforced

### Findings:

**Critical: 0**
**High: 0**
**Medium: 2**
1. Missing HSTS preload directive
2. Some endpoints lack rate limiting

**Low: 5**
- Informational headers could be improved
- Cookie SameSite attribute missing in some cases
- Consider implementing CSRF tokens
- Log retention policy not documented
- Backup encryption not verified

### Remediation Status:
- Medium issues: Fixed
- Low issues: Documented, will fix in Phase 2

## Penetration Test Results

- SQL Injection: ✓ Protected
- XSS: ✓ Protected
- CSRF: ✓ Protected (in critical endpoints)
- Authentication Bypass: ✓ Protected
- Authorization: ✓ Properly enforced
- Replay Attacks: ✓ Prevented
- Command Injection: ✓ Validated

## Compliance Readiness

- **POPIA (South Africa):** 85% ready
- **SOC2 Type I:** 70% ready (target: Phase 2)
- **ISO 27001:** 60% ready (target: Phase 3)

## Recommendations for Phase 2

1. Complete SOC2 Type I controls
2. Implement additional logging
3. Add anomaly detection ML models
4. Conduct quarterly penetration tests
5. Establish bug bounty program

## Conclusion

Smart Plug AI Phase 1 demonstrates strong security fundamentals and is 
approved for pilot deployment. Continued security focus in Phase 2 will 
ensure enterprise readiness.

**Auditor:** Internal Security Team  
**Date:** March 15, 2024  
**Next Review:** June 2024
```

---

## 🎯 WEEK 7 DELIVERABLES CHECKLIST

```text
Infrastructure Deliverables:
[√] Firebase production environment configured
[√] MQTT broker (EMQX) deployed with TLS 1.3
[√] Monitoring dashboard (Grafana) operational
[√] Alerting system (PagerDuty) integrated
[√] 24/7 monitoring active
[√] Backup procedures documented

Device Deliverables:
[√] 20 devices provisioned and tested
[√] Device IDs assigned (SP001-0001 to SP001-0020)
[√] ATECC608A secure elements provisioned
[√] Secure boot and flash encryption enabled
[√] QR pairing stickers applied
[√] Tamper-evident seals applied

User Onboarding:
[√] 20 pilot users selected
[√] Welcome kits sent
[√] User manuals distributed
[√] Initial training provided
[√] Support channels established
[√] Feedback collection system ready

Documentation:
[√] User Manual completed (50 pages)
[√] Developer Documentation updated
[√] Security Audit Report published
[√] API Documentation finalized
[√] Troubleshooting Guide created
[√] Setup guides for all components

Monitoring & Support:
[√] System health dashboard live
[√] Alert thresholds configured
[√] Incident response procedures documented
[√] Support ticket system ready
[√] 24/7 on-call rotation established

Quality Metrics Achieved:
[√] Uptime: 99.95% (target: 99.9%)
[√] API Response Time: < 150ms avg (target: < 200ms)
[√] Command Success Rate: 99.7% (target: 99%)
[√] Security Score: 92/100 (target: 85/100)
[√] User Satisfaction: 4.8/5.0 (pilot survey)

Phase 1 Complete:
✅ All 49 days of development completed
✅ 20 devices deployed to pilot users
✅ System monitoring 24/7
✅ Security audit passed
✅ Ready for Phase 2 scaling
```

---

## 🎊 PHASE 1 COMPLETION CELEBRATION

Congratulations! You've successfully completed Phase 1 of Smart Plug AI!

**What We've Achieved:**
- ✅ 3 secure smart plug prototypes built
- ✅ Mobile app with Firebase Auth + 2FA
- ✅ Web dashboard with real-time monitoring
- ✅ Secure backend with MQTT over TLS 1.3
- ✅ Command signing with ECDSA
- ✅ Rate limiting and anomaly detection
- ✅ Comprehensive test coverage (90%+)
- ✅ Security audit passed (Grade: A-)
- ✅ 20 pilot users onboarded
- ✅ 24/7 monitoring operational

**Next Steps (Phase 2 - Months 5-8):**
- Scale to 500 devices
- SABS/ICASA certification
- Manufacturing first batch
- Independent security audit
- Bug bounty program launch
- Enhanced features (energy disaggregation, reports)

**Thank you for following this detailed 7-week guide!**

Questions or feedback? Contact: dev@smartplugai.com

---

**End of Week 7 Guide** ✅  
**Phase 1 Complete!** 🎉  
**See you in Phase 2!** 🚀
