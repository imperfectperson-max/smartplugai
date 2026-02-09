# WEEK 3: BACKEND INTEGRATION & REAL DEVICE CONNECTION - DEEP DIVE

📅 **Timeline**: Days 15-21  
🎯 **Goal**: Connect physical ESP32 devices to cloud backend with secure MQTT/TLS communication and implement device attestation

## 📅 DAY 15-16: MQTT BROKER SETUP & DEVICE COMMUNICATION

### MORNING (9AM-12PM): Cloud MQTT Broker Configuration

#### Step 1: Set Up Google Cloud IoT Core / MQTT Broker

**Option A: Google Cloud IoT Core (Recommended for Production)**

```bash
# Install Google Cloud SDK
curl https://sdk.cloud.google.com | bash
exec -l $SHELL

# Initialize gcloud
gcloud init
gcloud auth login

# Enable IoT Core API
gcloud services enable cloudiot.googleapis.com
gcloud services enable cloudpubsub.googleapis.com

# Create Pub/Sub topics
gcloud pubsub topics create device-events
gcloud pubsub topics create device-commands

# Create Pub/Sub subscriptions
gcloud pubsub subscriptions create device-events-sub --topic=device-events
gcloud pubsub subscriptions create device-commands-sub --topic=device-commands

# Create IoT Core registry
gcloud iot registries create smartplug-registry \
    --project=YOUR_PROJECT_ID \
    --region=us-central1 \
    --event-notification-config=topic=device-events \
    --state-pubsub-topic=device-state

# Create device (per smart plug)
gcloud iot devices create smartplug-001 \
    --project=YOUR_PROJECT_ID \
    --region=us-central1 \
    --registry=smartplug-registry \
    --public-key path=./device_rsa_public.pem,type=rs256
```

**Option B: Self-Hosted Mosquitto MQTT Broker (Development)**

```bash
# Install Mosquitto on Ubuntu/Debian
sudo apt-get update
sudo apt-get install mosquitto mosquitto-clients

# Generate TLS certificates
cd /etc/mosquitto/certs

# Create CA certificate
openssl req -new -x509 -days 3650 -extensions v3_ca \
    -keyout ca.key -out ca.crt \
    -subj "/C=ZA/ST=WesternCape/L=CapeTown/O=SmartPlugAI/CN=CA"

# Create server certificate
openssl genrsa -out server.key 2048
openssl req -new -key server.key -out server.csr \
    -subj "/C=ZA/ST=WesternCape/L=CapeTown/O=SmartPlugAI/CN=mqtt.smartplugai.com"
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key \
    -CAcreateserial -out server.crt -days 3650

# Create client certificates (per device)
openssl genrsa -out device-001.key 2048
openssl req -new -key device-001.key -out device-001.csr \
    -subj "/C=ZA/ST=WesternCape/L=CapeTown/O=SmartPlugAI/CN=device-001"
openssl x509 -req -in device-001.csr -CA ca.crt -CAkey ca.key \
    -CAcreateserial -out device-001.crt -days 3650

# Set permissions
sudo chmod 600 *.key
sudo chmod 644 *.crt
```

**Configure Mosquitto with TLS**

**File**: `/etc/mosquitto/mosquitto.conf`

```conf
# Mosquitto Configuration - Smart Plug AI

# Network Configuration
listener 8883
protocol mqtt
max_connections 1000

# TLS Configuration
cafile /etc/mosquitto/certs/ca.crt
certfile /etc/mosquitto/certs/server.crt
keyfile /etc/mosquitto/certs/server.key
require_certificate true
use_identity_as_username true

# Persistence
persistence true
persistence_location /var/lib/mosquitto/

# Logging
log_dest file /var/log/mosquitto/mosquitto.log
log_type all
log_timestamp true

# Security
allow_anonymous false

# Performance
max_queued_messages 1000
message_size_limit 1048576

# Authentication (via plugin or file)
password_file /etc/mosquitto/passwd

# ACL (Access Control List)
acl_file /etc/mosquitto/acl
```

**Access Control List Configuration**

**File**: `/etc/mosquitto/acl`

```conf
# Admin users (backend services)
user backend-service
topic readwrite #

# Device permissions (per device)
user device-001
topic write devices/device-001/telemetry
topic write devices/device-001/status
topic read devices/device-001/commands
topic read devices/device-001/config

user device-002
topic write devices/device-002/telemetry
topic write devices/device-002/status
topic read devices/device-002/commands
topic read devices/device-002/config

# Pattern for all devices (adjust per your needs)
pattern read devices/%u/commands
pattern write devices/%u/telemetry
```

### AFTERNOON (1PM-5PM): ESP32 MQTT Client Implementation

#### Step 2: ESP32 Firmware MQTT Integration

**File**: `/firmware/main/mqtt_client.c`

```c
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include "cJSON.h"

static const char *TAG = "MQTT_CLIENT";

// External certificate (embedded in firmware)
extern const uint8_t ca_cert_pem_start[] asm("_binary_ca_crt_start");
extern const uint8_t ca_cert_pem_end[]   asm("_binary_ca_crt_end");
extern const uint8_t client_cert_pem_start[] asm("_binary_device_crt_start");
extern const uint8_t client_cert_pem_end[]   asm("_binary_device_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_device_key_start");
extern const uint8_t client_key_pem_end[]   asm("_binary_device_key_end");

static esp_mqtt_client_handle_t mqtt_client = NULL;

// MQTT topics
#define TELEMETRY_TOPIC "devices/%s/telemetry"
#define STATUS_TOPIC "devices/%s/status"
#define COMMAND_TOPIC "devices/%s/commands"
#define CONFIG_TOPIC "devices/%s/config"

// Device ID (unique per device, stored in ATECC608A or EEPROM)
static char device_id[32] = "device-001";

// Command handler function pointer
typedef void (*command_handler_t)(const char *command, cJSON *params);
static command_handler_t command_callback = NULL;

// MQTT event handler
static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            
            // Subscribe to command topic
            char command_topic[64];
            snprintf(command_topic, sizeof(command_topic), COMMAND_TOPIC, device_id);
            int msg_id = esp_mqtt_client_subscribe(client, command_topic, 1);
            ESP_LOGI(TAG, "Subscribed to %s, msg_id=%d", command_topic, msg_id);
            
            // Subscribe to config topic
            char config_topic[64];
            snprintf(config_topic, sizeof(config_topic), CONFIG_TOPIC, device_id);
            msg_id = esp_mqtt_client_subscribe(client, config_topic, 1);
            ESP_LOGI(TAG, "Subscribed to %s, msg_id=%d", config_topic, msg_id);
            
            // Publish online status
            mqtt_publish_status(true);
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
            
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            ESP_LOGI(TAG, "Topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Data: %.*s", event->data_len, event->data);
            
            // Parse and handle command
            handle_mqtt_command(event->data, event->data_len);
            break;
            
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGE(TAG, "Last errno: %d", event->error_handle->esp_tls_last_esp_err);
            }
            break;
            
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

// Initialize MQTT client
esp_err_t mqtt_client_init(const char *broker_url, const char *device_name,
                           command_handler_t callback)
{
    if (device_name) {
        strncpy(device_id, device_name, sizeof(device_id) - 1);
    }
    
    command_callback = callback;
    
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = broker_url,
        .broker.verification.certificate = (const char *)ca_cert_pem_start,
        .credentials = {
            .authentication = {
                .certificate = (const char *)client_cert_pem_start,
                .key = (const char *)client_key_pem_start,
            },
            .client_id = device_id,
        },
        .session.keepalive = 60,
        .session.disable_clean_session = false,
        .network.reconnect_timeout_ms = 10000,
        .network.timeout_ms = 10000,
    };
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }
    
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID,
                                   mqtt_event_handler, NULL);
    
    esp_err_t err = esp_mqtt_client_start(mqtt_client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MQTT client: %d", err);
        return err;
    }
    
    ESP_LOGI(TAG, "MQTT client initialized successfully");
    return ESP_OK;
}

// Publish telemetry data
esp_err_t mqtt_publish_telemetry(float voltage, float current, float power,
                                 float energy, float temperature)
{
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }
    
    // Create JSON payload
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "deviceId", device_id);
    cJSON_AddNumberToObject(root, "voltage", voltage);
    cJSON_AddNumberToObject(root, "current", current);
    cJSON_AddNumberToObject(root, "power", power);
    cJSON_AddNumberToObject(root, "energy", energy);
    cJSON_AddNumberToObject(root, "temperature", temperature);
    cJSON_AddNumberToObject(root, "timestamp", (double)time(NULL));
    
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    
    // Publish to telemetry topic
    char topic[64];
    snprintf(topic, sizeof(topic), TELEMETRY_TOPIC, device_id);
    
    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, json_string,
                                         strlen(json_string), 1, 0);
    
    free(json_string);
    
    if (msg_id < 0) {
        ESP_LOGE(TAG, "Failed to publish telemetry");
        return ESP_FAIL;
    }
    
    ESP_LOGD(TAG, "Telemetry published, msg_id=%d", msg_id);
    return ESP_OK;
}

// Publish device status
esp_err_t mqtt_publish_status(bool online)
{
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "deviceId", device_id);
    cJSON_AddStringToObject(root, "status", online ? "online" : "offline");
    cJSON_AddNumberToObject(root, "timestamp", (double)time(NULL));
    
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    
    char topic[64];
    snprintf(topic, sizeof(topic), STATUS_TOPIC, device_id);
    
    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, json_string,
                                         strlen(json_string), 1, 1); // Retain flag
    
    free(json_string);
    
    if (msg_id < 0) {
        ESP_LOGE(TAG, "Failed to publish status");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Status published, msg_id=%d", msg_id);
    return ESP_OK;
}

// Handle incoming MQTT commands
static void handle_mqtt_command(const char *data, int len)
{
    // Parse JSON command
    cJSON *root = cJSON_ParseWithLength(data, len);
    if (root == NULL) {
        ESP_LOGE(TAG, "Failed to parse command JSON");
        return;
    }
    
    cJSON *command_obj = cJSON_GetObjectItem(root, "command");
    if (command_obj == NULL || !cJSON_IsString(command_obj)) {
        ESP_LOGE(TAG, "Invalid command format");
        cJSON_Delete(root);
        return;
    }
    
    const char *command = command_obj->valuestring;
    cJSON *params = cJSON_GetObjectItem(root, "params");
    
    ESP_LOGI(TAG, "Received command: %s", command);
    
    // Call registered callback
    if (command_callback) {
        command_callback(command, params);
    }
    
    cJSON_Delete(root);
}

// Disconnect MQTT client
void mqtt_client_disconnect(void)
{
    if (mqtt_client) {
        mqtt_publish_status(false);
        esp_mqtt_client_stop(mqtt_client);
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
    }
}
```

#### Step 3: Main Application Integration

**File**: `/firmware/main/app_main.c`

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "mqtt_client.h"
#include "relay_control.h"
#include "current_sensor.h"
#include "atecc608a.h"

static const char *TAG = "MAIN";

// MQTT broker URL (update with your broker address)
#define MQTT_BROKER_URL "mqtts://mqtt.smartplugai.com:8883"

// Command handler
static void handle_device_command(const char *command, cJSON *params)
{
    ESP_LOGI(TAG, "Processing command: %s", command);
    
    if (strcmp(command, "turn_on") == 0) {
        relay_set_state(true);
        ESP_LOGI(TAG, "Device turned ON");
    }
    else if (strcmp(command, "turn_off") == 0) {
        relay_set_state(false);
        ESP_LOGI(TAG, "Device turned OFF");
    }
    else if (strcmp(command, "toggle") == 0) {
        relay_toggle();
        ESP_LOGI(TAG, "Device toggled");
    }
    else if (strcmp(command, "get_status") == 0) {
        // Publish current status
        float voltage, current, power;
        current_sensor_read(&voltage, &current, &power);
        mqtt_publish_telemetry(voltage, current, power, 0.0, 25.0);
    }
    else {
        ESP_LOGW(TAG, "Unknown command: %s", command);
    }
}

// Telemetry task - sends data every 5 seconds
static void telemetry_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Telemetry task started");
    
    while (1) {
        float voltage, current, power;
        float temperature = 25.0; // Read from temperature sensor if available
        float energy = 0.0; // Calculate cumulative energy
        
        // Read current sensor
        if (current_sensor_read(&voltage, &current, &power) == ESP_OK) {
            // Publish telemetry
            mqtt_publish_telemetry(voltage, current, power, energy, temperature);
        }
        
        // Wait 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Smart Plug AI - Firmware v1.0");
    
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Initialize WiFi (assuming WiFi credentials are stored)
    ESP_LOGI(TAG, "Initializing WiFi...");
    wifi_init();
    wifi_connect("YOUR_SSID", "YOUR_PASSWORD");
    
    // Wait for WiFi connection
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                           WIFI_CONNECTED_BIT,
                                           pdFALSE, pdTRUE, portMAX_DELAY);
    
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "WiFi connected successfully");
    }
    
    // Initialize hardware
    ESP_LOGI(TAG, "Initializing hardware...");
    relay_init();
    current_sensor_init();
    atecc608a_init();
    
    // Get device ID from ATECC608A serial number
    char device_id[32];
    atecc608a_get_serial(device_id, sizeof(device_id));
    ESP_LOGI(TAG, "Device ID: %s", device_id);
    
    // Initialize MQTT client
    ESP_LOGI(TAG, "Connecting to MQTT broker...");
    mqtt_client_init(MQTT_BROKER_URL, device_id, handle_device_command);
    
    // Start telemetry task
    xTaskCreate(telemetry_task, "telemetry_task", 4096, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Initialization complete");
}
```

---

## 📅 DAY 17-18: DEVICE ATTESTATION & SECURITY

### MORNING (9AM-12PM): Device Attestation Implementation

#### Step 4: ATECC608A Device Attestation

**File**: `/firmware/main/device_attestation.c`

```c
#include "atecc608a.h"
#include "mbedtls/sha256.h"
#include "mbedtls/ecdsa.h"
#include "esp_log.h"

static const char *TAG = "ATTESTATION";

/**
 * Generate attestation challenge response
 * 
 * This proves the device has the private key corresponding to its
 * certificate without revealing the private key.
 */
esp_err_t device_generate_attestation(const uint8_t *challenge, size_t challenge_len,
                                      uint8_t *signature, size_t *signature_len)
{
    ESP_LOGI(TAG, "Generating attestation for challenge");
    
    // Hash the challenge
    uint8_t challenge_hash[32];
    mbedtls_sha256_context sha256_ctx;
    mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts(&sha256_ctx, 0); // SHA-256 (not SHA-224)
    mbedtls_sha256_update(&sha256_ctx, challenge, challenge_len);
    mbedtls_sha256_finish(&sha256_ctx, challenge_hash);
    mbedtls_sha256_free(&sha256_ctx);
    
    // Sign challenge hash using ATECC608A private key (slot 0)
    uint8_t atecc_signature[64]; // ECDSA P256 signature (r||s)
    esp_err_t err = atecc608a_sign(0, challenge_hash, atecc_signature);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to sign challenge: %d", err);
        return err;
    }
    
    // Convert to DER format for transmission
    err = ecdsa_signature_to_der(atecc_signature, signature, signature_len);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to convert signature to DER: %d", err);
        return err;
    }
    
    ESP_LOGI(TAG, "Attestation generated successfully");
    return ESP_OK;
}

/**
 * Send attestation to backend
 */
esp_err_t device_attest_to_backend(void)
{
    ESP_LOGI(TAG, "Starting device attestation");
    
    // Step 1: Get device certificate
    uint8_t device_cert[512];
    size_t cert_len = sizeof(device_cert);
    esp_err_t err = atecc608a_get_certificate(device_cert, &cert_len);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get device certificate");
        return err;
    }
    
    // Step 2: Request challenge from backend via MQTT
    // (Backend sends a random 32-byte challenge)
    uint8_t challenge[32];
    err = mqtt_request_attestation_challenge(challenge, sizeof(challenge));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get attestation challenge");
        return err;
    }
    
    // Step 3: Generate attestation signature
    uint8_t signature[128];
    size_t signature_len = sizeof(signature);
    err = device_generate_attestation(challenge, sizeof(challenge),
                                      signature, &signature_len);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to generate attestation");
        return err;
    }
    
    // Step 4: Send attestation response to backend
    err = mqtt_send_attestation_response(device_cert, cert_len,
                                         signature, signature_len);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send attestation response");
        return err;
    }
    
    ESP_LOGI(TAG, "Device attestation completed successfully");
    return ESP_OK;
}
```

### AFTERNOON (1PM-5PM): Backend Attestation Verification

#### Step 5: Cloud Function for Attestation Verification

**File**: `/backend/functions/src/deviceAttestation.ts`

```typescript
import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import * as crypto from 'crypto';
import { X509Certificate } from 'crypto';

const db = admin.firestore();

// Device attestation request
export const requestAttestation = functions.https.onCall(async (data, context) => {
  if (!context.auth) {
    throw new functions.https.HttpsError('unauthenticated', 'User must be authenticated');
  }
  
  const { deviceId } = data;
  
  // Generate random challenge
  const challenge = crypto.randomBytes(32).toString('base64');
  
  // Store challenge with expiration (5 minutes)
  await db.collection('attestationChallenges').doc(deviceId).set({
    challenge,
    userId: context.auth.uid,
    createdAt: admin.firestore.FieldValue.serverTimestamp(),
    expiresAt: new Date(Date.now() + 5 * 60 * 1000),
  });
  
  return { challenge };
});

// Verify device attestation
export const verifyAttestation = functions.https.onCall(async (data, context) => {
  if (!context.auth) {
    throw new functions.https.HttpsError('unauthenticated', 'User must be authenticated');
  }
  
  const { deviceId, certificate, signature } = data;
  
  try {
    // Get stored challenge
    const challengeDoc = await db.collection('attestationChallenges').doc(deviceId).get();
    if (!challengeDoc.exists) {
      throw new functions.https.HttpsError('not-found', 'Challenge not found');
    }
    
    const challengeData = challengeDoc.data();
    if (!challengeData || new Date() > challengeData.expiresAt.toDate()) {
      throw new functions.https.HttpsError('deadline-exceeded', 'Challenge expired');
    }
    
    const challenge = Buffer.from(challengeData.challenge, 'base64');
    
    // Parse device certificate
    const cert = new X509Certificate(certificate);
    
    // Extract public key from certificate
    const publicKey = crypto.createPublicKey({
      key: certificate,
      format: 'pem',
    });
    
    // Verify signature
    const verify = crypto.createVerify('SHA256');
    verify.update(challenge);
    verify.end();
    
    const isValid = verify.verify(publicKey, Buffer.from(signature, 'base64'));
    
    if (!isValid) {
      throw new functions.https.HttpsError('permission-denied', 'Invalid attestation');
    }
    
    // Extract device info from certificate
    const serialNumber = cert.serialNumber;
    const subject = cert.subject;
    
    // Register device as attested
    await db.collection('devices').doc(deviceId).update({
      attested: true,
      attestedAt: admin.firestore.FieldValue.serverTimestamp(),
      certificate: certificate,
      serialNumber,
      ownerId: context.auth.uid,
    });
    
    // Delete challenge
    await db.collection('attestationChallenges').doc(deviceId).delete();
    
    return {
      success: true,
      message: 'Device attested successfully',
      deviceId,
    };
  } catch (error) {
    console.error('Attestation verification error:', error);
    throw new functions.https.HttpsError('internal', 'Attestation verification failed');
  }
});
```

---

## 📅 DAY 19-21: END-TO-END DATA FLOW & TESTING

### MORNING (9AM-12PM): Complete Data Pipeline

#### Step 6: Firestore Cloud Function for Device Data Processing

**File**: `/backend/functions/src/dataProcessing.ts`

```typescript
import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';

const db = admin.firestore();

// Process incoming telemetry data
export const processTelemetry = functions.firestore
  .document('rawTelemetry/{telemetryId}')
  .onCreate(async (snapshot, context) => {
    const telemetry = snapshot.data();
    const deviceId = telemetry.deviceId;
    
    try {
      // Validate device exists and is attested
      const deviceDoc = await db.collection('devices').doc(deviceId).get();
      if (!deviceDoc.exists || !deviceDoc.data()?.attested) {
        console.warn(`Rejecting telemetry from unattested device: ${deviceId}`);
        return null;
      }
      
      // Store processed telemetry
      await db
        .collection('devices')
        .doc(deviceId)
        .collection('telemetry')
        .add({
          voltage: telemetry.voltage,
          current: telemetry.current,
          power: telemetry.power,
          energy: telemetry.energy,
          temperature: telemetry.temperature,
          timestamp: admin.firestore.FieldValue.serverTimestamp(),
        });
      
      // Update device current state
      await db.collection('devices').doc(deviceId).update({
        currentVoltage: telemetry.voltage,
        currentCurrent: telemetry.current,
        currentPower: telemetry.power,
        currentTemperature: telemetry.temperature,
        lastSeenAt: admin.firestore.FieldValue.serverTimestamp(),
        isOnline: true,
      });
      
      // Calculate daily energy consumption
      const today = new Date();
      today.setHours(0, 0, 0, 0);
      
      const dailyStatsDoc = await db
        .collection('devices')
        .doc(deviceId)
        .collection('dailyStats')
        .doc(today.toISOString().split('T')[0])
        .get();
      
      const currentEnergy = dailyStatsDoc.exists
        ? dailyStatsDoc.data()?.totalEnergy || 0
        : 0;
      
      await db
        .collection('devices')
        .doc(deviceId)
        .collection('dailyStats')
        .doc(today.toISOString().split('T')[0])
        .set({
          totalEnergy: currentEnergy + (telemetry.energy || 0),
          lastUpdated: admin.firestore.FieldValue.serverTimestamp(),
        }, { merge: true });
      
      // Check for alerts
      if (telemetry.power > 2000) {
        await db.collection('alerts').add({
          deviceId,
          type: 'high_power',
          severity: 'warning',
          message: `High power usage: ${telemetry.power}W`,
          timestamp: admin.firestore.FieldValue.serverTimestamp(),
          acknowledged: false,
        });
      }
      
      if (telemetry.temperature > 60) {
        await db.collection('alerts').add({
          deviceId,
          type: 'high_temperature',
          severity: 'critical',
          message: `High temperature: ${telemetry.temperature}°C`,
          timestamp: admin.firestore.FieldValue.serverTimestamp(),
          acknowledged: false,
        });
      }
      
      return null;
    } catch (error) {
      console.error('Error processing telemetry:', error);
      return null;
    }
  });

// Monitor device online status
export const monitorDeviceStatus = functions.pubsub
  .schedule('every 1 minutes')
  .onRun(async (context) => {
    const fiveMinutesAgo = new Date(Date.now() - 5 * 60 * 1000);
    
    // Find devices that haven't sent data in 5 minutes
    const devicesSnapshot = await db
      .collection('devices')
      .where('lastSeenAt', '<', fiveMinutesAgo)
      .where('isOnline', '==', true)
      .get();
    
    const batch = db.batch();
    
    devicesSnapshot.forEach((doc) => {
      batch.update(doc.ref, {
        isOnline: false,
        lastOfflineAt: admin.firestore.FieldValue.serverTimestamp(),
      });
    });
    
    await batch.commit();
    
    console.log(`Marked ${devicesSnapshot.size} devices as offline`);
    return null;
  });
```

### AFTERNOON (1PM-5PM): Integration Testing

#### Step 7: End-to-End Testing Script

**File**: `/tests/integration_test.py`

```python
#!/usr/bin/env python3
"""
End-to-end integration test for Smart Plug AI
Tests the complete flow: Device → MQTT → Backend → Frontend
"""

import time
import json
import paho.mqtt.client as mqtt
import requests
from firebase_admin import credentials, firestore, initialize_app

# Configuration
MQTT_BROKER = "mqtt.smartplugai.com"
MQTT_PORT = 8883
DEVICE_ID = "test-device-001"
TEST_USER_TOKEN = "YOUR_FIREBASE_AUTH_TOKEN"

# Initialize Firebase Admin
cred = credentials.Certificate("serviceAccountKey.json")
initialize_app(cred)
db = firestore.client()

def test_mqtt_connection():
    """Test MQTT connection with TLS"""
    print("Testing MQTT connection...")
    
    client = mqtt.Client(client_id=DEVICE_ID)
    client.tls_set(
        ca_certs="certs/ca.crt",
        certfile="certs/device-001.crt",
        keyfile="certs/device-001.key"
    )
    
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("✓ MQTT connection successful")
        else:
            print(f"✗ MQTT connection failed with code {rc}")
    
    client.on_connect = on_connect
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()
    time.sleep(2)
    client.loop_stop()
    client.disconnect()

def test_telemetry_publish():
    """Test telemetry data publishing"""
    print("Testing telemetry publishing...")
    
    client = mqtt.Client(client_id=DEVICE_ID)
    client.tls_set(
        ca_certs="certs/ca.crt",
        certfile="certs/device-001.crt",
        keyfile="certs/device-001.key"
    )
    
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    
    telemetry = {
        "deviceId": DEVICE_ID,
        "voltage": 230.5,
        "current": 0.5,
        "power": 115.25,
        "energy": 0.1,
        "temperature": 28.5,
        "timestamp": time.time()
    }
    
    result = client.publish(
        f"devices/{DEVICE_ID}/telemetry",
        json.dumps(telemetry),
        qos=1
    )
    
    if result.rc == mqtt.MQTT_ERR_SUCCESS:
        print("✓ Telemetry published successfully")
    else:
        print(f"✗ Failed to publish telemetry: {result.rc}")
    
    client.disconnect()

def test_firestore_data():
    """Test that data appears in Firestore"""
    print("Testing Firestore data storage...")
    
    time.sleep(3)  # Wait for Cloud Function to process
    
    device_ref = db.collection('devices').document(DEVICE_ID)
    device = device_ref.get()
    
    if device.exists:
        data = device.to_dict()
        if data.get('currentPower') is not None:
            print(f"✓ Data stored in Firestore: Power = {data['currentPower']}W")
        else:
            print("✗ Data not found in Firestore")
    else:
        print("✗ Device not found in Firestore")

def test_command_flow():
    """Test sending command from backend to device"""
    print("Testing command flow...")
    
    # Subscribe to command topic
    received_command = []
    
    def on_message(client, userdata, message):
        payload = json.loads(message.payload.decode())
        received_command.append(payload)
        print(f"✓ Received command: {payload}")
    
    client = mqtt.Client(client_id=DEVICE_ID)
    client.tls_set(
        ca_certs="certs/ca.crt",
        certfile="certs/device-001.crt",
        keyfile="certs/device-001.key"
    )
    client.on_message = on_message
    
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.subscribe(f"devices/{DEVICE_ID}/commands", qos=1)
    client.loop_start()
    
    # Publish command (simulating backend)
    command = {
        "command": "turn_on",
        "timestamp": time.time(),
        "nonce": "test123"
    }
    
    client.publish(
        f"devices/{DEVICE_ID}/commands",
        json.dumps(command),
        qos=1
    )
    
    time.sleep(2)
    client.loop_stop()
    client.disconnect()
    
    if received_command:
        print("✓ Command flow successful")
    else:
        print("✗ Command not received")

def run_all_tests():
    """Run all integration tests"""
    print("=" * 60)
    print("Smart Plug AI - Integration Tests")
    print("=" * 60)
    
    tests = [
        test_mqtt_connection,
        test_telemetry_publish,
        test_firestore_data,
        test_command_flow,
    ]
    
    for test in tests:
        try:
            test()
            print()
        except Exception as e:
            print(f"✗ Test failed: {e}\n")
    
    print("=" * 60)
    print("Tests completed")
    print("=" * 60)

if __name__ == "__main__":
    run_all_tests()
```

---

## 🧪 TESTING CHECKLIST

### MQTT Communication Tests
```
[ ] MQTT broker accepts TLS connections
[ ] Device can connect with client certificate
[ ] Device can publish telemetry data
[ ] Device can subscribe to command topic
[ ] Commands are received by device
[ ] Connection is maintained (keepalive)
[ ] Reconnection works after disconnect
```

### Device Attestation Tests
```
[ ] Device can request attestation challenge
[ ] Device can sign challenge with ATECC608A
[ ] Backend can verify attestation signature
[ ] Invalid attestation is rejected
[ ] Unauthenticated devices cannot publish data
```

### Data Flow Tests
```
[ ] Telemetry data reaches Firestore
[ ] Device state updates in real-time
[ ] Dashboard displays current readings
[ ] Historical data is stored correctly
[ ] Alerts are generated for anomalies
```

### Security Tests
```
[ ] TLS encryption is enforced
[ ] Certificate validation works
[ ] Access control list (ACL) is enforced
[ ] Unauthorized topics are blocked
[ ] Rate limiting prevents spam
```

---

## 📊 WEEK 3 SUCCESS CRITERIA

### Infrastructure Deliverables
```
[ ] MQTT broker deployed and configured
[ ] TLS certificates generated and distributed
[ ] Access control lists configured
[ ] Cloud Functions deployed
[ ] Firestore collections created
[ ] Security rules updated
```

### Firmware Deliverables
```
[ ] MQTT client library integrated
[ ] Device attestation implemented
[ ] Telemetry publishing working
[ ] Command handling functional
[ ] Automatic reconnection working
[ ] Error handling robust
```

### Backend Deliverables
```
[ ] Device attestation verification
[ ] Telemetry processing pipeline
[ ] Command routing to devices
[ ] Alert generation system
[ ] Device status monitoring
[ ] Data aggregation for analytics
```

### Integration Deliverables
```
[ ] End-to-end data flow working
[ ] Real-time dashboard updates
[ ] Mobile app shows live data
[ ] Commands executed on devices
[ ] All tests passing
```

---

## ⚠️ COMMON ISSUES & SOLUTIONS

### MQTT Connection Issues
```
Problem: "TLS handshake failed"
Solution: Verify certificate chain is correct
         Check that CA certificate matches server certificate
         Ensure client certificate is signed by correct CA
         Check certificate expiration dates

Problem: "Connection refused"
Solution: Verify MQTT broker is running
         Check firewall rules allow port 8883
         Verify DNS resolution for broker hostname
         Check ACL allows device to connect
```

### Data Not Appearing in Firestore
```
Problem: Telemetry published but not in Firestore
Solution: Check Cloud Function logs for errors
         Verify Firestore security rules allow writes
         Check device is attested
         Verify Cloud Function is deployed
         Check Pub/Sub subscriptions are active
```

---

## 📞 RESOURCES & NEXT STEPS

### Documentation
- MQTT Protocol: https://mqtt.org/
- ESP-IDF MQTT: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html
- Firebase Cloud Functions: https://firebase.google.com/docs/functions

### Next Week Preview
Week 4 will focus on safety features, PCB design, 3D enclosure creation, and preparing for production manufacturing.

---

**Version**: 1.0  
**Last Updated**: February 2026
