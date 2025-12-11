# Firmware

Coming soon — firmware for the ESP32-based smart plug.

This directory will contain the ESP32 firmware, sensor drivers (INA219 or SCT-013), relay control code, and OTA update support. Initial tasks to add here:

- Basic ESP32 sketch to read INA219 and publish MQTT messages
- Relay control and safety checks (fuse, temperature cutoff)
- OTA update support and build instructions
- Wiring diagram and PCB notes

Reference: The top-level README lists "Firmware" as coming soon; use this file as the canonical place for firmware resources and example sketches.
