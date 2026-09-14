#include "wifi_manager.h"

#include <WiFi.h>
#include <WiFiManager.h>

namespace {
WiFiManager wifiManager;
bool wifiConnected = false;
}

namespace WifiManager {
void begin() {
    WiFi.mode(WIFI_STA);
    wifiConnected = wifiManager.autoConnect("CAN-OWM-Modbus-Setup");
}

void loop() {
    wifiConnected = WiFi.status() == WL_CONNECTED;
}

bool isConnected() {
    return wifiConnected;
}
}