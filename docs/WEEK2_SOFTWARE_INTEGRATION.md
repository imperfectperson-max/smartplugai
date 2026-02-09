# WEEK 2: SOFTWARE INTEGRATION & MOBILE APP - DEEP DIVE

📅 **Timeline**: Days 8-14  
🎯 **Goal**: Complete mobile app and web dashboard with Firebase authentication and real-time data integration

## 📅 DAY 8-9: MOBILE APP FINALIZATION

### MORNING (9AM-12PM): Flutter App Authentication Setup

#### Step 1: Firebase Authentication Configuration

```text
Checklist - Firebase Setup:
[√] Create Firebase project in console
[√] Enable Firebase Authentication
[√] Enable Email/Password authentication
[√] Enable Phone authentication (for 2FA)
[√] Configure OAuth providers (Google, Apple)
[√] Download google-services.json (Android)
[√] Download GoogleService-Info.plist (iOS)
[√] Add Firebase SDK to Flutter project
```

**Firebase Console Setup Diagram:**
```
Firebase Console
├─ Authentication
│  ├─ Sign-in method
│  │  ├─ [✓] Email/Password
│  │  ├─ [✓] Phone
│  │  ├─ [✓] Google
│  │  └─ [✓] Apple (iOS only)
│  └─ Settings
│     └─ Authorized domains (add your domain)
│
├─ Firestore Database
│  ├─ Create database (production mode)
│  ├─ Set security rules
│  └─ Create collections:
│     ├─ users/
│     ├─ devices/
│     └─ readings/
│
└─ Cloud Functions
   ├─ Enable billing
   └─ Deploy functions later
```

#### Step 2: Implement Authentication Flow

**File**: `/app/lib/services/auth_service.dart`

```dart
import 'package:firebase_auth/firebase_auth.dart';
import 'package:cloud_firestore/cloud_firestore.dart';

class AuthService {
  final FirebaseAuth _auth = FirebaseAuth.instance;
  final FirebaseFirestore _firestore = FirebaseFirestore.instance;
  
  // Email/Password Registration
  Future<UserCredential> registerWithEmail(String email, String password) async {
    try {
      UserCredential userCredential = await _auth.createUserWithEmailAndPassword(
        email: email,
        password: password,
      );
      
      // Create user document in Firestore
      await _firestore.collection('users').doc(userCredential.user!.uid).set({
        'email': email,
        'createdAt': FieldValue.serverTimestamp(),
        'twoFactorEnabled': false,
        'devices': [],
      });
      
      // Send verification email
      await userCredential.user!.sendEmailVerification();
      
      return userCredential;
    } on FirebaseAuthException catch (e) {
      throw _handleAuthException(e);
    }
  }
  
  // Email/Password Login
  Future<UserCredential> loginWithEmail(String email, String password) async {
    try {
      return await _auth.signInWithEmailAndPassword(
        email: email,
        password: password,
      );
    } on FirebaseAuthException catch (e) {
      throw _handleAuthException(e);
    }
  }
  
  // Enable 2FA (Phone verification)
  Future<void> enableTwoFactor(String phoneNumber) async {
    await _auth.verifyPhoneNumber(
      phoneNumber: phoneNumber,
      verificationCompleted: (PhoneAuthCredential credential) async {
        await _auth.currentUser!.updatePhoneNumber(credential);
      },
      verificationFailed: (FirebaseAuthException e) {
        throw Exception('Phone verification failed: ${e.message}');
      },
      codeSent: (String verificationId, int? resendToken) {
        // Store verificationId for later use
      },
      codeAutoRetrievalTimeout: (String verificationId) {},
    );
  }
  
  // Logout
  Future<void> logout() async {
    await _auth.signOut();
  }
  
  // Error handler
  String _handleAuthException(FirebaseAuthException e) {
    switch (e.code) {
      case 'weak-password':
        return 'Password is too weak. Use at least 8 characters.';
      case 'email-already-in-use':
        return 'An account already exists for this email.';
      case 'user-not-found':
        return 'No user found with this email.';
      case 'wrong-password':
        return 'Incorrect password.';
      default:
        return 'Authentication error: ${e.message}';
    }
  }
}
```

#### Step 3: Build Login and Registration Screens

**File**: `/app/lib/screens/auth/login_screen.dart`

```dart
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../../services/auth_service.dart';

class LoginScreen extends StatefulWidget {
  @override
  _LoginScreenState createState() => _LoginScreenState();
}

class _LoginScreenState extends State<LoginScreen> {
  final _formKey = GlobalKey<FormState>();
  final _emailController = TextEditingController();
  final _passwordController = TextEditingController();
  bool _isLoading = false;
  String _errorMessage = '';
  
  Future<void> _handleLogin() async {
    if (!_formKey.currentState!.validate()) return;
    
    setState(() {
      _isLoading = true;
      _errorMessage = '';
    });
    
    try {
      final authService = Provider.of<AuthService>(context, listen: false);
      await authService.loginWithEmail(
        _emailController.text.trim(),
        _passwordController.text,
      );
      
      // Navigate to home screen
      Navigator.pushReplacementNamed(context, '/home');
    } catch (e) {
      setState(() {
        _errorMessage = e.toString();
        _isLoading = false;
      });
    }
  }
  
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Smart Plug AI - Login'),
        backgroundColor: Colors.blue[700],
      ),
      body: Padding(
        padding: EdgeInsets.all(24.0),
        child: Form(
          key: _formKey,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Logo
              Icon(Icons.power_settings_new, size: 80, color: Colors.blue[700]),
              SizedBox(height: 32),
              
              // Email field
              TextFormField(
                controller: _emailController,
                decoration: InputDecoration(
                  labelText: 'Email',
                  border: OutlineInputBorder(),
                  prefixIcon: Icon(Icons.email),
                ),
                keyboardType: TextInputType.emailAddress,
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'Please enter your email';
                  }
                  if (!value.contains('@')) {
                    return 'Please enter a valid email';
                  }
                  return null;
                },
              ),
              SizedBox(height: 16),
              
              // Password field
              TextFormField(
                controller: _passwordController,
                decoration: InputDecoration(
                  labelText: 'Password',
                  border: OutlineInputBorder(),
                  prefixIcon: Icon(Icons.lock),
                ),
                obscureText: true,
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'Please enter your password';
                  }
                  if (value.length < 8) {
                    return 'Password must be at least 8 characters';
                  }
                  return null;
                },
              ),
              SizedBox(height: 24),
              
              // Error message
              if (_errorMessage.isNotEmpty)
                Padding(
                  padding: EdgeInsets.only(bottom: 16),
                  child: Text(
                    _errorMessage,
                    style: TextStyle(color: Colors.red),
                  ),
                ),
              
              // Login button
              ElevatedButton(
                onPressed: _isLoading ? null : _handleLogin,
                child: _isLoading
                    ? CircularProgressIndicator(color: Colors.white)
                    : Text('Login'),
                style: ElevatedButton.styleFrom(
                  minimumSize: Size(double.infinity, 50),
                ),
              ),
              SizedBox(height: 16),
              
              // Register link
              TextButton(
                onPressed: () {
                  Navigator.pushNamed(context, '/register');
                },
                child: Text('Don\'t have an account? Register'),
              ),
            ],
          ),
        ),
      ),
    );
  }
  
  @override
  void dispose() {
    _emailController.dispose();
    _passwordController.dispose();
    super.dispose();
  }
}
```

### AFTERNOON (1PM-5PM): Device Pairing UI

#### Step 4: Device Discovery and Pairing Screen

**File**: `/app/lib/screens/devices/device_pairing_screen.dart`

```dart
import 'package:flutter/material.dart';
import 'package:wifi_iot/wifi_iot.dart';
import '../../services/device_service.dart';

class DevicePairingScreen extends StatefulWidget {
  @override
  _DevicePairingScreenState createState() => _DevicePairingScreenState();
}

class _DevicePairingScreenState extends State<DevicePairingScreen> {
  List<WifiNetwork> _availableNetworks = [];
  bool _isScanning = false;
  String _statusMessage = 'Searching for Smart Plugs...';
  
  @override
  void initState() {
    super.initState();
    _startDeviceDiscovery();
  }
  
  Future<void> _startDeviceDiscovery() async {
    setState(() {
      _isScanning = true;
    });
    
    try {
      // Scan for WiFi networks starting with "SmartPlug-"
      final networks = await WiFiForIoTPlugin.loadWifiList();
      setState(() {
        _availableNetworks = networks
            .where((network) => network.ssid?.startsWith('SmartPlug-') ?? false)
            .toList();
        _isScanning = false;
        _statusMessage = _availableNetworks.isEmpty
            ? 'No devices found. Make sure your Smart Plug is powered on.'
            : 'Found ${_availableNetworks.length} device(s)';
      });
    } catch (e) {
      setState(() {
        _isScanning = false;
        _statusMessage = 'Error scanning: $e';
      });
    }
  }
  
  Future<void> _pairDevice(WifiNetwork network) async {
    // Show pairing dialog
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text('Pair Device'),
        content: Text('Connecting to ${network.ssid}...'),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: Text('Cancel'),
          ),
        ],
      ),
    );
    
    try {
      // Connect to device AP
      await WiFiForIoTPlugin.connect(
        network.ssid!,
        password: 'smartplug123', // Default password
        security: NetworkSecurity.WPA,
      );
      
      // Send WiFi credentials to device
      final deviceService = DeviceService();
      await deviceService.provisionDevice(
        deviceSSID: network.ssid!,
        homeWiFiSSID: 'YOUR_HOME_WIFI',
        homeWiFiPassword: 'YOUR_WIFI_PASSWORD',
      );
      
      Navigator.pop(context);
      Navigator.pushNamed(context, '/home');
    } catch (e) {
      Navigator.pop(context);
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Pairing failed: $e')),
      );
    }
  }
  
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Pair New Device'),
        actions: [
          IconButton(
            icon: Icon(Icons.refresh),
            onPressed: _isScanning ? null : _startDeviceDiscovery,
          ),
        ],
      ),
      body: Column(
        children: [
          // Status banner
          Container(
            width: double.infinity,
            padding: EdgeInsets.all(16),
            color: Colors.blue[50],
            child: Row(
              children: [
                if (_isScanning)
                  Padding(
                    padding: EdgeInsets.only(right: 16),
                    child: CircularProgressIndicator(),
                  ),
                Expanded(child: Text(_statusMessage)),
              ],
            ),
          ),
          
          // Device list
          Expanded(
            child: _availableNetworks.isEmpty
                ? Center(
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Icon(Icons.power_off, size: 64, color: Colors.grey),
                        SizedBox(height: 16),
                        Text(
                          'No devices found',
                          style: TextStyle(fontSize: 18, color: Colors.grey),
                        ),
                        SizedBox(height: 8),
                        Text(
                          'Make sure your Smart Plug is powered on\nand in pairing mode',
                          textAlign: TextAlign.center,
                          style: TextStyle(color: Colors.grey),
                        ),
                        SizedBox(height: 24),
                        ElevatedButton.icon(
                          onPressed: _startDeviceDiscovery,
                          icon: Icon(Icons.refresh),
                          label: Text('Scan Again'),
                        ),
                      ],
                    ),
                  )
                : ListView.builder(
                    itemCount: _availableNetworks.length,
                    itemBuilder: (context, index) {
                      final network = _availableNetworks[index];
                      return ListTile(
                        leading: Icon(Icons.router, size: 40),
                        title: Text(network.ssid ?? 'Unknown'),
                        subtitle: Text('Signal: ${network.level}%'),
                        trailing: ElevatedButton(
                          onPressed: () => _pairDevice(network),
                          child: Text('Pair'),
                        ),
                      );
                    },
                  ),
          ),
        ],
      ),
    );
  }
}
```

---

## 📅 DAY 10-11: WEB DASHBOARD DEVELOPMENT

### MORNING (9AM-12PM): React Dashboard Setup

#### Step 1: Firebase Web Configuration

**File**: `/web/src/config/firebase.js`

```javascript
import { initializeApp } from 'firebase/app';
import { getAuth } from 'firebase/auth';
import { getFirestore } from 'firebase/firestore';
import { getFunctions } from 'firebase/functions';

const firebaseConfig = {
  apiKey: process.env.REACT_APP_FIREBASE_API_KEY,
  authDomain: process.env.REACT_APP_FIREBASE_AUTH_DOMAIN,
  projectId: process.env.REACT_APP_FIREBASE_PROJECT_ID,
  storageBucket: process.env.REACT_APP_FIREBASE_STORAGE_BUCKET,
  messagingSenderId: process.env.REACT_APP_FIREBASE_MESSAGING_SENDER_ID,
  appId: process.env.REACT_APP_FIREBASE_APP_ID,
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Initialize services
export const auth = getAuth(app);
export const db = getFirestore(app);
export const functions = getFunctions(app);

export default app;
```

#### Step 2: Real-Time Device Dashboard

**File**: `/web/src/components/Dashboard.jsx`

```javascript
import React, { useState, useEffect } from 'react';
import { collection, onSnapshot, query, where } from 'firebase/firestore';
import { db, auth } from '../config/firebase';
import DeviceCard from './DeviceCard';
import EnergyChart from './EnergyChart';

function Dashboard() {
  const [devices, setDevices] = useState([]);
  const [loading, setLoading] = useState(true);
  const [totalPower, setTotalPower] = useState(0);
  
  useEffect(() => {
    const user = auth.currentUser;
    if (!user) return;
    
    // Subscribe to user's devices
    const q = query(
      collection(db, 'devices'),
      where('ownerId', '==', user.uid)
    );
    
    const unsubscribe = onSnapshot(q, (snapshot) => {
      const devicesData = snapshot.docs.map(doc => ({
        id: doc.id,
        ...doc.data()
      }));
      
      setDevices(devicesData);
      
      // Calculate total power
      const total = devicesData.reduce((sum, device) => 
        sum + (device.currentPower || 0), 0
      );
      setTotalPower(total);
      setLoading(false);
    });
    
    return () => unsubscribe();
  }, []);
  
  if (loading) {
    return (
      <div className="flex justify-center items-center h-screen">
        <div className="animate-spin rounded-full h-32 w-32 border-b-2 border-blue-500"></div>
      </div>
    );
  }
  
  return (
    <div className="container mx-auto px-4 py-8">
      <h1 className="text-3xl font-bold mb-8">Smart Plug Dashboard</h1>
      
      {/* Summary Cards */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-8">
        <div className="bg-white rounded-lg shadow-md p-6">
          <h3 className="text-gray-500 text-sm font-medium">Total Devices</h3>
          <p className="text-3xl font-bold text-blue-600">{devices.length}</p>
        </div>
        
        <div className="bg-white rounded-lg shadow-md p-6">
          <h3 className="text-gray-500 text-sm font-medium">Active Devices</h3>
          <p className="text-3xl font-bold text-green-600">
            {devices.filter(d => d.isOn).length}
          </p>
        </div>
        
        <div className="bg-white rounded-lg shadow-md p-6">
          <h3 className="text-gray-500 text-sm font-medium">Total Power</h3>
          <p className="text-3xl font-bold text-orange-600">
            {totalPower.toFixed(2)} W
          </p>
        </div>
      </div>
      
      {/* Energy Chart */}
      <div className="bg-white rounded-lg shadow-md p-6 mb-8">
        <h2 className="text-xl font-semibold mb-4">Energy Usage</h2>
        <EnergyChart devices={devices} />
      </div>
      
      {/* Device Grid */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {devices.map(device => (
          <DeviceCard key={device.id} device={device} />
        ))}
      </div>
      
      {devices.length === 0 && (
        <div className="text-center py-12">
          <p className="text-gray-500 mb-4">No devices found</p>
          <button className="bg-blue-500 text-white px-6 py-2 rounded-lg hover:bg-blue-600">
            Add Your First Device
          </button>
        </div>
      )}
    </div>
  );
}

export default Dashboard;
```

### AFTERNOON (1PM-5PM): Device Control Components

#### Step 3: Device Card with Real-Time Control

**File**: `/web/src/components/DeviceCard.jsx`

```javascript
import React, { useState } from 'react';
import { doc, updateDoc } from 'firebase/firestore';
import { db } from '../config/firebase';
import { Power, Zap, Clock } from 'lucide-react';

function DeviceCard({ device }) {
  const [loading, setLoading] = useState(false);
  
  const toggleDevice = async () => {
    setLoading(true);
    try {
      const deviceRef = doc(db, 'devices', device.id);
      await updateDoc(deviceRef, {
        isOn: !device.isOn,
        lastToggled: new Date(),
      });
    } catch (error) {
      console.error('Error toggling device:', error);
      alert('Failed to toggle device');
    } finally {
      setLoading(false);
    }
  };
  
  const formatUptime = (seconds) => {
    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    return `${hours}h ${minutes}m`;
  };
  
  return (
    <div className={`bg-white rounded-lg shadow-md p-6 border-l-4 ${
      device.isOn ? 'border-green-500' : 'border-gray-300'
    }`}>
      {/* Header */}
      <div className="flex justify-between items-start mb-4">
        <div>
          <h3 className="text-lg font-semibold">{device.name}</h3>
          <p className="text-sm text-gray-500">{device.location}</p>
        </div>
        <button
          onClick={toggleDevice}
          disabled={loading}
          className={`p-3 rounded-full transition-colors ${
            device.isOn
              ? 'bg-green-500 hover:bg-green-600'
              : 'bg-gray-300 hover:bg-gray-400'
          } ${loading ? 'opacity-50 cursor-not-allowed' : ''}`}
        >
          <Power className="w-6 h-6 text-white" />
        </button>
      </div>
      
      {/* Status */}
      <div className="space-y-3">
        <div className="flex items-center justify-between">
          <div className="flex items-center space-x-2">
            <Zap className={`w-5 h-5 ${device.isOn ? 'text-yellow-500' : 'text-gray-400'}`} />
            <span className="text-sm text-gray-600">Power</span>
          </div>
          <span className="font-semibold text-lg">
            {device.currentPower?.toFixed(1) || '0.0'} W
          </span>
        </div>
        
        <div className="flex items-center justify-between">
          <div className="flex items-center space-x-2">
            <Clock className="w-5 h-5 text-blue-500" />
            <span className="text-sm text-gray-600">Uptime</span>
          </div>
          <span className="text-sm font-medium">
            {formatUptime(device.uptime || 0)}
          </span>
        </div>
        
        <div className="pt-3 border-t border-gray-200">
          <div className="flex justify-between text-sm">
            <span className="text-gray-600">Today's Energy</span>
            <span className="font-semibold">
              {(device.todayEnergy || 0).toFixed(2)} kWh
            </span>
          </div>
        </div>
      </div>
      
      {/* Status indicator */}
      <div className="mt-4 flex items-center space-x-2">
        <div className={`w-2 h-2 rounded-full ${
          device.isOnline ? 'bg-green-500' : 'bg-red-500'
        }`}></div>
        <span className="text-xs text-gray-500">
          {device.isOnline ? 'Online' : 'Offline'}
        </span>
      </div>
    </div>
  );
}

export default DeviceCard;
```

---

## 📅 DAY 12-14: BACKEND INTEGRATION PREP

### MORNING (9AM-12PM): Firestore Security Rules

#### Step 1: Configure Firestore Security Rules

**File**: `/backend/firestore.rules`

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
    
    function isDeviceOwner() {
      return isAuthenticated() && 
             request.auth.uid == resource.data.ownerId;
    }
    
    // Users collection
    match /users/{userId} {
      allow read, write: if isOwner(userId);
      
      // Allow users to read their own device list
      allow read: if isAuthenticated() && 
                     request.auth.uid == userId;
    }
    
    // Devices collection
    match /devices/{deviceId} {
      // Users can read their own devices
      allow read: if isAuthenticated() && 
                     resource.data.ownerId == request.auth.uid;
      
      // Users can create new devices
      allow create: if isAuthenticated() &&
                       request.resource.data.ownerId == request.auth.uid;
      
      // Users can update their own devices
      allow update: if isDeviceOwner();
      
      // Users can delete their own devices
      allow delete: if isDeviceOwner();
    }
    
    // Readings collection (device data)
    match /readings/{deviceId}/data/{readingId} {
      // Users can read readings from their devices
      allow read: if isAuthenticated() &&
                     get(/databases/$(database)/documents/devices/$(deviceId)).data.ownerId == request.auth.uid;
      
      // Only backend can write readings (via Cloud Functions)
      allow write: if false;
    }
    
    // Automation rules
    match /automations/{automationId} {
      allow read, write: if isAuthenticated() &&
                            resource.data.userId == request.auth.uid;
    }
  }
}
```

### AFTERNOON (1PM-5PM): Cloud Functions Setup

#### Step 2: Device Command Cloud Function

**File**: `/backend/functions/src/deviceControl.ts`

```typescript
import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import { PubSub } from '@google-cloud/pubsub';

const pubsub = new PubSub();
const db = admin.firestore();

// Send command to device via MQTT/Pub/Sub
export const sendDeviceCommand = functions.https.onCall(async (data, context) => {
  // Verify authentication
  if (!context.auth) {
    throw new functions.https.HttpsError(
      'unauthenticated',
      'User must be authenticated'
    );
  }
  
  const { deviceId, command, value } = data;
  
  // Validate input
  if (!deviceId || !command) {
    throw new functions.https.HttpsError(
      'invalid-argument',
      'Missing required fields'
    );
  }
  
  try {
    // Verify user owns device
    const deviceRef = db.collection('devices').doc(deviceId);
    const deviceDoc = await deviceRef.get();
    
    if (!deviceDoc.exists) {
      throw new functions.https.HttpsError(
        'not-found',
        'Device not found'
      );
    }
    
    const deviceData = deviceDoc.data();
    if (deviceData?.ownerId !== context.auth.uid) {
      throw new functions.https.HttpsError(
        'permission-denied',
        'You do not own this device'
      );
    }
    
    // Create command payload
    const commandPayload = {
      deviceId,
      command,
      value,
      timestamp: admin.firestore.FieldValue.serverTimestamp(),
      userId: context.auth.uid,
      nonce: Math.random().toString(36).substring(7), // Anti-replay
    };
    
    // Publish command to device topic
    const topicName = `device-commands-${deviceId}`;
    const dataBuffer = Buffer.from(JSON.stringify(commandPayload));
    
    await pubsub.topic(topicName).publish(dataBuffer);
    
    // Log command in Firestore
    await db.collection('commandLogs').add({
      ...commandPayload,
      status: 'sent',
    });
    
    return {
      success: true,
      message: 'Command sent successfully',
      commandId: commandPayload.nonce,
    };
  } catch (error) {
    console.error('Error sending command:', error);
    throw new functions.https.HttpsError(
      'internal',
      'Failed to send command'
    );
  }
});

// Process device readings
export const processDeviceReading = functions.pubsub
  .topic('device-readings')
  .onPublish(async (message) => {
    const reading = message.json;
    
    try {
      // Store reading in Firestore
      await db
        .collection('readings')
        .doc(reading.deviceId)
        .collection('data')
        .add({
          ...reading,
          timestamp: admin.firestore.FieldValue.serverTimestamp(),
        });
      
      // Update device status
      await db.collection('devices').doc(reading.deviceId).update({
        currentPower: reading.power,
        currentVoltage: reading.voltage,
        currentCurrent: reading.current,
        lastSeen: admin.firestore.FieldValue.serverTimestamp(),
        isOnline: true,
      });
      
      // Check for alerts (high power, etc.)
      if (reading.power > 2000) {
        await db.collection('alerts').add({
          deviceId: reading.deviceId,
          type: 'high_power',
          message: `High power usage detected: ${reading.power}W`,
          timestamp: admin.firestore.FieldValue.serverTimestamp(),
        });
      }
      
      return null;
    } catch (error) {
      console.error('Error processing reading:', error);
      return null;
    }
  });
```

---

## 🧪 TESTING CHECKLIST

### Pre-Integration Testing
```
[ ] Firebase project created and configured
[ ] Authentication working (email/password)
[ ] 2FA setup and testing
[ ] Firestore collections created
[ ] Security rules deployed and tested
[ ] Mobile app compiles without errors
[ ] Web dashboard loads correctly
[ ] All environment variables configured
```

### Integration Testing
```
[ ] User registration flow works end-to-end
[ ] Login and logout functional
[ ] Device pairing UI working
[ ] Dashboard displays mock data
[ ] Real-time updates working (Firestore listeners)
[ ] Device control buttons functional
[ ] Error handling works correctly
[ ] Loading states display properly
```

### Security Testing
```
[ ] Firestore security rules prevent unauthorized access
[ ] API keys stored securely (environment variables)
[ ] HTTPS enforced on all connections
[ ] Password complexity requirements met
[ ] Email verification working
[ ] Session management secure
```

---

## 📊 WEEK 2 SUCCESS CRITERIA

### Mobile App Deliverables
```
[ ] Complete authentication system (email, 2FA)
[ ] Device pairing interface
[ ] Home dashboard with device list
[ ] Real-time power monitoring UI
[ ] Settings and profile screens
[ ] Error handling and user feedback
[ ] iOS and Android builds successful
```

### Web Dashboard Deliverables
```
[ ] Responsive dashboard layout
[ ] Real-time device status display
[ ] Device control interface
[ ] Energy usage charts
[ ] User profile management
[ ] Security settings (2FA, password change)
[ ] Production build optimized
```

### Backend Deliverables
```
[ ] Firebase project fully configured
[ ] Firestore security rules deployed
[ ] Cloud Functions initialized
[ ] Device command function tested
[ ] Reading processing function tested
[ ] Authentication endpoints secured
[ ] API rate limiting configured
```

---

## ⚠️ COMMON ISSUES & SOLUTIONS

### Firebase Authentication Issues
```
Problem: "Firebase app not initialized"
Solution: Check that google-services.json is in the correct location
         Verify Firebase SDK is added in pubspec.yaml (Flutter)
         Run 'flutter clean' and rebuild

Problem: "User not authenticated"
Solution: Check auth state listener is properly set up
         Verify Firebase Auth is enabled in console
         Check security rules allow authenticated users
```

### Real-Time Updates Not Working
```
Problem: Dashboard not updating automatically
Solution: Verify Firestore listener is active (onSnapshot)
         Check Firestore security rules allow read access
         Inspect browser console for errors
         Test network connectivity
```

### Device Pairing Failures
```
Problem: Cannot find device WiFi network
Solution: Ensure device is in pairing mode (LED blinking)
         Check phone WiFi permissions granted
         Verify device is powered on
         Try manual WiFi connection first
```

---

## 📞 RESOURCES & NEXT STEPS

### Documentation
- Flutter Firebase: https://firebase.flutter.dev/
- React Firebase: https://firebase.google.com/docs/web/setup
- Firestore Security Rules: https://firebase.google.com/docs/firestore/security/get-started

### Next Week Preview
Week 3 will focus on connecting real ESP32 devices to the backend via MQTT/TLS, implementing device attestation, and building the complete end-to-end data flow.

---

**Remember**: Security first! Always validate user input and test security rules thoroughly.

**Version**: 1.0  
**Last Updated**: February 2026
