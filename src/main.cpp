#include <Arduino.h>
#include <LittleFS.h>

#include "app_config.h"
#include "config.h"
#include "modbus_server.h"
#include "owm_client.h"
#include "wifi_manager.h"
#include "web_server.h"

namespace {
OWMClient owmClient;
ModbusServer modbusServer;
unsigned long lastWeatherFetch = 0;
AppConfig::Data appConfig;
}

void setup() {
    Serial.begin(115200);
    delay(200);

    pinMode(Config::HEARTBEAT_LED_PIN, OUTPUT);
    pinMode(Config::SYSTEM_LED_PIN, OUTPUT);

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS mount failed");
    }

    appConfig = AppConfig::defaults();
    AppConfig::load(appConfig);

    WifiManager::begin();
    owmClient.begin(appConfig.owmApiKey, appConfig.owmLocation);
    modbusServer.begin();
    WebServerApp::begin();
}

void loop() {
    static unsigned long lastBlink = 0;
    const unsigned long now = millis();

    if (now - lastBlink >= 1000) {
        lastBlink = now;
        digitalWrite(Config::HEARTBEAT_LED_PIN, !digitalRead(Config::HEARTBEAT_LED_PIN));
    }

    WifiManager::loop();
    modbusServer.loop();

    if (WifiManager::isConnected() && now - lastWeatherFetch >= 30UL * 60UL * 1000UL) {
        lastWeatherFetch = now;
        if (owmClient.fetchWeather()) {
            modbusServer.updateSunshineHours(
                owmClient.getSunshineHours(Config::SUN_INTERVAL_1D_MS),
                owmClient.getSunshineHours(Config::SUN_INTERVAL_2D_MS),
                owmClient.getSunshineHours(Config::SUN_INTERVAL_3D_MS)
            );
        }
    }

    WebServerApp::loop();
    delay(10);
}