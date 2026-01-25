# Firmware Examples - Week 1

This directory contains example firmware code with detailed comments for Week 1 hardware assembly and firmware development.

## 📁 Files

### atecc608a_driver.c
Complete ATECC608A secure element driver with:
- I2C initialization (GPIO8/GPIO9)
- Device initialization and provisioning checks
- Key generation examples
- Signing and verification
- Safety warnings and checklists

### current_sensor_driver.c
SCT-013-030 current sensor driver with:
- ADC configuration and calibration
- RMS current calculation
- True RMS sampling (10kHz)
- Calibration procedures
- Noise filtering

### relay_control.c
Relay control with PC817 optocoupler:
- GPIO initialization
- Soft-start for inductive loads
- Safety interlocks
- State management

### watchdog_tamper.c
TPS3823 watchdog and tamper detection:
- Watchdog toggle task
- Tamper ISR handler
- Safe shutdown procedures
- Enclosure monitoring

## 🎯 Week 1 Usage

These examples are designed to be:
1. **Educational**: Learn how each component works
2. **Reference**: Copy-paste starting points for your firmware
3. **Tested**: Based on actual Week 1 hardware configuration
4. **Safe**: Include critical warnings and checklists

## 🔧 Integration

To use these examples in your ESP-IDF project:

```bash
# Copy to your project components
cp examples/*.c components/drivers/

# Update CMakeLists.txt
# Add to your component's source files
```

## ⚠️ Important Notes

- **Test thoroughly** before using in production
- **Check pin assignments** match your hardware
- **Follow safety checklists** at the end of each file
- **Don't lock ATECC608A** until fully tested
- **Calibrate current sensor** with known loads

## 📚 Related Documentation

- [Week 1 Hardware Mastery](../docs/WEEK1_HARDWARE_MASTERY.md)
- [Soldering Checklist](../docs/soldering-checklist.md)
- [Quick Reference](../docs/WEEK1_QUICK_REFERENCE.md)
- [Firmware README](../README.md)

## 🐛 Troubleshooting

If examples don't work:
1. Check wiring matches Week 1 diagrams
2. Verify I2C pull-ups are installed (2.2kΩ)
3. Test components individually before integration
4. Check ESP-IDF version (v5.2.1 recommended)
5. Consult troubleshooting section in each file

---

**Version**: 1.0  
**Last Updated**: January 2026  
**Compatible with**: ESP-IDF v5.2.1, ESP32-S3
