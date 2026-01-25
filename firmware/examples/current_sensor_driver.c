/**
 * @file current_sensor_driver.c
 * @brief SCT-013-030 Current Sensor Driver for ESP32-S3
 * 
 * This driver reads AC current using the SCT-013-030 non-invasive
 * current transformer and calculates True RMS current values.
 * 
 * WEEK 1 Implementation Notes:
 * - Connected to GPIO4 (ADC1_CH3)
 * - Uses 33Ω 1W burden resistor
 * - Protection diode (1N4007) across burden resistor
 * - 100nF noise filter capacitor
 * - Sampling at 10kHz for accurate RMS calculation
 * 
 * Circuit Diagram:
 * ```
 * SCT-013-030 TIP → [33Ω] → GPIO4 (ADC)
 *                    [1N4007] Protection
 *                    [100nF]  Filter
 *                      ↓
 *                     GND
 * ```
 * 
 * Calibration Required:
 * - Zero offset: No-load voltage reading
 * - Scale factor: Volts per Ampere ratio
 * - Tested with 60W and 100W incandescent bulbs
 * 
 * @date January 2026
 * @version 1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <math.h>

static const char *TAG = "CURRENT_SENSOR";

// Hardware configuration (Week 1 setup)
#define CURRENT_SENSOR_ADC_CHANNEL  ADC1_CHANNEL_3  /*!< GPIO4 = ADC1_CH3 */
#define ADC_ATTEN                   ADC_ATTEN_DB_11 /*!< 0-3.3V range */
#define ADC_WIDTH                   ADC_WIDTH_BIT_12 /*!< 12-bit resolution */

// Sampling configuration
#define SAMPLES_PER_CYCLE          200              /*!< 10kHz sampling for 50Hz */
#define LINE_FREQUENCY             50.0             /*!< AC frequency in Hz */
#define BURDEN_RESISTOR            33.0             /*!< Burden resistor in Ω */
#define SENSOR_RATIO               30.0             /*!< 30A → 1V output */

/**
 * @brief Calibration data structure
 * 
 * Stores calibration constants for accurate current measurement:
 * - zero_offset: ADC voltage reading at 0A (typically ~1.65V)
 * - scale_factor: Voltage per Ampere conversion
 * - temperature_coeff: Compensation for temperature drift
 * - calibration_time: When calibration was performed
 */
typedef struct {
    float zero_offset;           /*!< ADC reading at 0A (volts) */
    float scale_factor;          /*!< V/A conversion factor */
    float phase_correction;      /*!< Phase angle correction (degrees) */
    float temperature_coeff;     /*!< Temperature compensation */
    uint32_t calibration_time;   /*!< Unix timestamp of calibration */
} calibration_t;

static calibration_t calib = {
    .zero_offset = 1.65,         // Default mid-scale for 3.3V ADC
    .scale_factor = 0.0333,      // 33Ω × (1V/30A) = 0.0333 V/A
    .phase_correction = 0.0,
    .temperature_coeff = 0.001,
    .calibration_time = 0
};

static esp_adc_cal_characteristics_t *adc_chars = NULL;

/**
 * @brief Initialize current sensor and ADC
 * 
 * Setup sequence:
 * 1. Configure ADC width and attenuation
 * 2. Characterize ADC for accurate voltage readings
 * 3. Load calibration data from EEPROM (if available)
 * 
 * @return ESP_OK on success
 */
esp_err_t current_sensor_init(void)
{
    ESP_LOGI(TAG, "Initializing SCT-013-030 current sensor...");
    
    // Configure ADC width
    esp_err_t err = adc1_config_width(ADC_WIDTH);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ADC width config failed: %s", esp_err_to_name(err));
        return err;
    }
    
    // Configure ADC attenuation for channel
    err = adc1_config_channel_atten(CURRENT_SENSOR_ADC_CHANNEL, ADC_ATTEN);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ADC atten config failed: %s", esp_err_to_name(err));
        return err;
    }
    
    // Characterize ADC for accurate measurements
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    if (adc_chars == NULL) {
        ESP_LOGE(TAG, "Failed to allocate ADC characteristics");
        return ESP_ERR_NO_MEM;
    }
    
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
        ADC_UNIT_1, 
        ADC_ATTEN, 
        ADC_WIDTH,
        1100,  // Default Vref (will be measured during calibration)
        adc_chars
    );
    
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        ESP_LOGI(TAG, "ADC characterized using Two Point values");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        ESP_LOGI(TAG, "ADC characterized using eFuse Vref");
    } else {
        ESP_LOGW(TAG, "ADC characterized using Default Vref");
    }
    
    // TODO: Load calibration from EEPROM
    // load_calibration_from_eeprom(&calib);
    
    ESP_LOGI(TAG, "Current sensor initialized");
    ESP_LOGI(TAG, "Zero offset: %.3fV, Scale: %.4f V/A", 
             calib.zero_offset, calib.scale_factor);
    
    return ESP_OK;
}

/**
 * @brief Read True RMS current value
 * 
 * Algorithm:
 * 1. Sample one complete AC cycle (200 samples @ 10kHz for 50Hz)
 * 2. Convert ADC readings to voltages
 * 3. Calculate RMS: sqrt(mean(V²))
 * 4. Convert voltage to current using calibration
 * 
 * Timing:
 * - One cycle @ 50Hz = 20ms
 * - 200 samples → 100µs between samples (10kHz)
 * - Total read time: ~20ms
 * 
 * @return RMS current in Amperes
 */
float read_current_rms(void)
{
    int32_t samples[SAMPLES_PER_CYCLE];
    
    // Sample one complete cycle at 10kHz
    for (int i = 0; i < SAMPLES_PER_CYCLE; i++) {
        samples[i] = adc1_get_raw(CURRENT_SENSOR_ADC_CHANNEL);
        ets_delay_us(100);  // 100µs delay → 10kHz sampling
    }
    
    // Convert samples to voltages and calculate sum of squares
    float sum_squares = 0;
    for (int i = 0; i < SAMPLES_PER_CYCLE; i++) {
        // Convert ADC reading to voltage (in mV, then to V)
        uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(samples[i], adc_chars);
        float voltage_v = voltage_mv / 1000.0;
        
        // Subtract zero offset (AC signal centered at Vcc/2)
        float ac_voltage = voltage_v - calib.zero_offset;
        
        // Add to sum of squares
        sum_squares += ac_voltage * ac_voltage;
    }
    
    // Calculate RMS voltage
    float voltage_rms = sqrt(sum_squares / SAMPLES_PER_CYCLE);
    
    // Convert to current using calibration
    // Current = Voltage / (Burden Resistor × Sensor Ratio)
    // With calibration: Current = Voltage / scale_factor
    float current = voltage_rms / calib.scale_factor;
    
    return current;
}

/**
 * @brief Calibrate zero offset (no-load condition)
 * 
 * This must be performed with NO AC load connected!
 * 
 * Procedure:
 * 1. Disconnect all AC loads
 * 2. Wait 5 seconds for system to stabilize
 * 3. Take 1000 samples over 1 second
 * 4. Calculate average as zero offset
 * 5. Save to EEPROM
 * 
 * Expected result: ~1.65V (mid-scale for 3.3V ADC)
 * 
 * ⚠️ WARNING: Ensure NO AC load before calibrating!
 * 
 * @return ESP_OK on successful calibration
 */
esp_err_t calibrate_zero_offset(void)
{
    ESP_LOGI(TAG, "=== ZERO OFFSET CALIBRATION ===");
    ESP_LOGI(TAG, "Ensure NO LOAD connected to smart plug!");
    ESP_LOGI(TAG, "Waiting 5 seconds for system to stabilize...");
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    ESP_LOGI(TAG, "Sampling...");
    
    float sum = 0;
    const int num_samples = 1000;
    
    for (int i = 0; i < num_samples; i++) {
        int raw = adc1_get_raw(CURRENT_SENSOR_ADC_CHANNEL);
        uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, adc_chars);
        sum += voltage_mv / 1000.0;
        vTaskDelay(pdMS_TO_TICKS(1));  // 1ms between samples
    }
    
    calib.zero_offset = sum / num_samples;
    calib.calibration_time = 0;  // TODO: Get actual timestamp
    
    ESP_LOGI(TAG, "Zero offset calibrated: %.3fV", calib.zero_offset);
    
    // Sanity check
    if (calib.zero_offset < 1.5 || calib.zero_offset > 1.8) {
        ESP_LOGW(TAG, "Zero offset outside expected range (1.5-1.8V)");
        ESP_LOGW(TAG, "Check circuit: burden resistor, capacitor, connections");
    }
    
    // TODO: Save to EEPROM
    // save_calibration_to_eeprom(&calib);
    
    return ESP_OK;
}

/**
 * @brief Calibrate with known load
 * 
 * Calibration procedure with reference load:
 * 1. Connect known load (e.g., 60W incandescent bulb)
 * 2. Calculate expected current: I = P/V (60W/230V = 0.26A)
 * 3. Measure actual voltage from sensor
 * 4. Calculate scale factor: V/A
 * 5. Save to EEPROM
 * 
 * Example loads:
 * - 60W bulb @ 230V = 0.261A
 * - 100W bulb @ 230V = 0.435A
 * - Kettle 2000W @ 230V = 8.70A
 * 
 * @param expected_current Expected current in Amperes
 * @return ESP_OK on successful calibration
 */
esp_err_t calibrate_with_known_load(float expected_current)
{
    ESP_LOGI(TAG, "=== KNOWN LOAD CALIBRATION ===");
    ESP_LOGI(TAG, "Expected current: %.3fA", expected_current);
    ESP_LOGI(TAG, "Waiting 3 seconds for load to stabilize...");
    
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    // Take multiple RMS readings
    float sum_voltage = 0;
    const int num_readings = 10;
    
    for (int i = 0; i < num_readings; i++) {
        // Read raw RMS voltage (before scale factor applied)
        int32_t samples[SAMPLES_PER_CYCLE];
        for (int j = 0; j < SAMPLES_PER_CYCLE; j++) {
            samples[j] = adc1_get_raw(CURRENT_SENSOR_ADC_CHANNEL);
            ets_delay_us(100);
        }
        
        float sum_squares = 0;
        for (int j = 0; j < SAMPLES_PER_CYCLE; j++) {
            uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(samples[j], adc_chars);
            float voltage_v = voltage_mv / 1000.0;
            float ac_voltage = voltage_v - calib.zero_offset;
            sum_squares += ac_voltage * ac_voltage;
        }
        
        float voltage_rms = sqrt(sum_squares / SAMPLES_PER_CYCLE);
        sum_voltage += voltage_rms;
        
        vTaskDelay(pdMS_TO_TICKS(100));  // 100ms between readings
    }
    
    float measured_voltage = sum_voltage / num_readings;
    
    // Calculate scale factor: V/A
    calib.scale_factor = measured_voltage / expected_current;
    calib.calibration_time = 0;  // TODO: Get actual timestamp
    
    ESP_LOGI(TAG, "Measured voltage: %.3fV", measured_voltage);
    ESP_LOGI(TAG, "Scale factor: %.4f V/A", calib.scale_factor);
    
    // Sanity check (for 33Ω burden, expect ~0.033 V/A)
    if (calib.scale_factor < 0.020 || calib.scale_factor > 0.050) {
        ESP_LOGW(TAG, "Scale factor outside expected range (0.020-0.050 V/A)");
        ESP_LOGW(TAG, "Verify burden resistor value and sensor ratio");
    }
    
    // TODO: Save to EEPROM
    // save_calibration_to_eeprom(&calib);
    
    return ESP_OK;
}

/**
 * @brief Verify calibration accuracy
 * 
 * Test calibration with second known load:
 * 1. Measure current with calibrated sensor
 * 2. Compare to expected current
 * 3. Calculate error percentage
 * 4. Pass if error < 2%
 * 
 * @param expected_current Expected current for test load
 * @return ESP_OK if error < 2%
 */
esp_err_t verify_calibration(float expected_current)
{
    ESP_LOGI(TAG, "=== CALIBRATION VERIFICATION ===");
    ESP_LOGI(TAG, "Expected: %.3fA", expected_current);
    
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // Take 10 measurements
    float sum_current = 0;
    for (int i = 0; i < 10; i++) {
        float current = read_current_rms();
        sum_current += current;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    float measured_current = sum_current / 10;
    float error = fabs(measured_current - expected_current);
    float error_percent = (error / expected_current) * 100;
    
    ESP_LOGI(TAG, "Measured: %.3fA", measured_current);
    ESP_LOGI(TAG, "Error: %.3fA (%.2f%%)", error, error_percent);
    
    if (error_percent < 2.0) {
        ESP_LOGI(TAG, "✓ Calibration PASSED (error < 2%%)");
        return ESP_OK;
    } else {
        ESP_LOGW(TAG, "✗ Calibration FAILED (error >= 2%%)");
        ESP_LOGW(TAG, "Consider recalibrating or checking connections");
        return ESP_FAIL;
    }
}

/**
 * Week 1 Safety Checklist:
 * 
 * Before using this driver:
 * [ ] Burden resistor installed (33Ω 1W)
 * [ ] Protection diode installed (1N4007 across burden)
 * [ ] Filter capacitor installed (100nF to GND)
 * [ ] ADC pin connected to GPIO4
 * [ ] SCT-013 properly clamped around AC line
 * [ ] White stripe on SCT-013 facing correct direction
 * [ ] Zero offset calibrated with no load
 * [ ] Scale factor calibrated with known load
 * [ ] Verification test passed (<2% error)
 * 
 * Common Issues:
 * - Reading 0A always: Check burden resistor connection
 * - Negative readings: Flip SCT-013 clamp direction
 * - Noisy readings: Check filter capacitor, improve grounding
 * - Inaccurate readings: Re-calibrate with precision reference load
 * - Readings drift: Check temperature coefficient, power supply stability
 * 
 * Troubleshooting:
 * - Measure DC voltage at GPIO4 with no load (should be ~1.65V)
 * - Check AC voltage across burden resistor (should vary with load)
 * - Verify ADC reading updates when load changes
 * - Test with multiple known loads to verify linearity
 */
