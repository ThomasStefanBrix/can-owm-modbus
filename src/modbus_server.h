#pragma once

#include <Arduino.h>
#include <ModbusIP_ESP8266.h>

class ModbusServer {
public:
    ModbusServer();

    void begin(uint16_t port = 502);
    void loop();
    void updateSunshineHours(float oneDay, float twoDay, float threeDay);

    uint16_t readRegister(uint16_t address) const;
    void writeRegister(uint16_t address, uint16_t value);

private:
    ModbusIP modbus;
    uint16_t registers[256] = {};

    static uint16_t encodeHours(float hours);
    void syncRegister(uint16_t address, uint16_t value);
};
