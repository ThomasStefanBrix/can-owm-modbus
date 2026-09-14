#include "modbus_server.h"

#include "config.h"

ModbusServer::ModbusServer() = default;

void ModbusServer::begin(uint16_t port) {
    modbus.server(port);
    modbus.addHreg(Config::ModbusRegister::SUN_1D, 0);
    modbus.addHreg(Config::ModbusRegister::SUN_2D, 0);
    modbus.addHreg(Config::ModbusRegister::SUN_3D, 0);
}

void ModbusServer::loop() {
    modbus.task();
}

void ModbusServer::updateSunshineHours(float oneDay, float twoDay, float threeDay) {
    syncRegister(Config::ModbusRegister::SUN_1D, encodeHours(oneDay));
    syncRegister(Config::ModbusRegister::SUN_2D, encodeHours(twoDay));
    syncRegister(Config::ModbusRegister::SUN_3D, encodeHours(threeDay));
}

uint16_t ModbusServer::readRegister(uint16_t address) const {
    return address < 256 ? registers[address] : 0;
}

void ModbusServer::writeRegister(uint16_t address, uint16_t value) {
    if (address >= 256) {
        return;
    }

    syncRegister(address, value);
}

uint16_t ModbusServer::encodeHours(float hours) {
    if (hours < 0.0f) {
        hours = 0.0f;
    }

    if (hours > 655.35f) {
        hours = 655.35f;
    }

    return static_cast<uint16_t>(hours * 100.0f + 0.5f);
}

void ModbusServer::syncRegister(uint16_t address, uint16_t value) {
    if (address >= 256) {
        return;
    }

    registers[address] = value;
    modbus.Hreg(address, value);
}
