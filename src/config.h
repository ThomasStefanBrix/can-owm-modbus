#pragma once

#include <Arduino.h>
#include <cstddef>
#include <stdint.h>

namespace Config {
static constexpr uint8_t CAN_TX_PIN = 11;
static constexpr uint8_t CAN_RX_PIN = 10;
static constexpr uint8_t RS485_TX_PIN = 16;
static constexpr uint8_t RS485_RX_PIN = 15;
static constexpr uint8_t RS485_RTS_PIN = 17;
static constexpr uint8_t HEARTBEAT_LED_PIN = 2;
static constexpr uint8_t SYSTEM_LED_PIN = 48;
static constexpr uint32_t CAN_BAUDRATE = 500000;
static constexpr uint32_t RS485_BAUDRATE = 9600;
static constexpr char WIFI_CONFIG_FILE[] = "/wifi_config.json";
static constexpr char WEATHER_API_URL[] = "https://api.openweathermap.org/data/2.5/weather?q=Tromso,no&appid=YOUR_API_KEY&units=metric";
static constexpr char OWM_API_KEY[] = "YOUR_API_KEY";
static constexpr char OWM_LOCATION[] = "Tromso,no";
static constexpr uint32_t SUN_INTERVAL_1D_MS = 24UL * 60UL * 60UL * 1000UL;
static constexpr uint32_t SUN_INTERVAL_2D_MS = 2UL * SUN_INTERVAL_1D_MS;
static constexpr uint32_t SUN_INTERVAL_3D_MS = 3UL * SUN_INTERVAL_1D_MS;

static constexpr size_t NUM_DEVICES = 3;

struct Device {
	uint16_t canId;
	const char* name;
};

extern const Device DEVICES[NUM_DEVICES];

namespace ModbusRegister {
static constexpr uint16_t SUN_1D = 100;
static constexpr uint16_t SUN_2D = 101;
static constexpr uint16_t SUN_3D = 102;
}
}