#include "web_server.h"

#include <WebServer.h>

#include "wifi_manager.h"

namespace {
WebServer server(80);
}

namespace WebServerApp {
void begin() {
    server.on("/", []() {
        server.send(200, "text/plain", WifiManager::isConnected() ? "WiFi connected" : "WiFi provisioning active");
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
}