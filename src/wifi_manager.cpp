#include "wifi_manager.h"

#include <WiFi.h>
#include <WiFiManager.h>

namespace {
WiFiManager wifiManager;
bool wifiConnected = false;
bool ipReported = false;
}

namespace WifiManager {
void begin() {
    WiFi.mode(WIFI_STA);
    wifiConnected = wifiManager.autoConnect("CAN-OWM-Modbus-Setup");
    ipReported = false;

    if (wifiConnected) {
        Serial.print("WiFi connected, IP: ");
        Serial.println(WiFi.localIP());
        ipReported = true;
    } else {
        Serial.println("WiFi provisioning active");
    }
}

void loop() {
    const bool connected = WiFi.status() == WL_CONNECTED;

    if (connected && !wifiConnected && !ipReported) {
        Serial.print("WiFi connected, IP: ");
        Serial.println(WiFi.localIP());
        ipReported = true;
    }

    wifiConnected = connected;
}

bool isConnected() {
    return wifiConnected;
}

String getLocalIP() {
    return wifiConnected ? WiFi.localIP().toString() : String();
}
}