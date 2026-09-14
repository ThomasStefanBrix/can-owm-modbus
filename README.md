# CAN-OWM-Modbus Arduino

Arduino-based starter project for the Waveshare ESP32-S3-RS485-CAN board.

## Included stack

- WiFiManager for WiFi provisioning
- ArduinoJson for configuration and API responses
- LittleFS for local storage
- WebServer for a lightweight web UI
- HTTPClient for weather data
- modbus-esp8266 for Modbus TCP

## Recommended board setup

- Board: ESP32-S3 DevKitC-1
- Target hardware: Waveshare ESP32-S3-RS485-CAN
- Flash size: 8 MB
- PSRAM: enabled

## Project layout

- `src/main.cpp` entry point
- `src/wifi_manager.*` WiFi setup and provisioning
- `src/web_server.*` web endpoints
- `src/can_gateway.*` CAN/TWAI integration
- `src/config.h` shared constants

## Build

Use PlatformIO in VS Code and run the default build task for the environment `waveshare-esp32-s3-rs485-can`.

## Notes

This is a clean Arduino replacement for the archived ESP-IDF version.

The OpenWeatherMap API key and location are loaded from `/app_config.json` in LittleFS. If the file does not exist, the defaults from `src/config.h` are used.

## Modbus register map

Sunshine hours are stored as fixed-point values with two decimal places.

- Register 100: 1-day sunshine hours
- Register 101: 2-day sunshine hours
- Register 102: 3-day sunshine hours

Example: `12.34` hours is stored as `1234`.
