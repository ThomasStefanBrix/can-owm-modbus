#pragma once

#include <Arduino.h>

namespace WifiManager {
void begin();
void loop();
bool isConnected();
String getLocalIP();
}