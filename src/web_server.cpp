#include "web_server.h"

#include <WebServer.h>

#include "config.h"
#include "owm_client.h"
#include "wifi_manager.h"

namespace {
WebServer server(80);
OWMClient* owmClient = nullptr;
}

namespace WebServerApp {
void setOwmClient(OWMClient* client) {
    owmClient = client;
}

void begin() {
    server.on("/status", []() {
        String response;
        response.reserve(128);
        response += "WiFi: ";
        response += WifiManager::isConnected() ? "connected" : "provisioning";
        response += "\nIP: ";
        response += WifiManager::isConnected() ? WifiManager::getLocalIP() : "-";
        response += "\nEndpoint: / and /ip";
        server.send(200, "text/plain", response);
    });

    server.on("/", []() {
        String html;
        html.reserve(256);
        html += "<!doctype html><html><head><meta charset='utf-8'>";
        html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
        html += "<title>CAN-OWM-Modbus</title></head><body style='font-family:sans-serif'>";
        html += "<h1>CAN-OWM-Modbus</h1>";
        html += "<p>WiFi: ";
        html += WifiManager::isConnected() ? "connected" : "provisioning";
        html += "</p><p>IP: ";
        html += WifiManager::isConnected() ? WifiManager::getLocalIP() : "-";
        html += "</p><p><a href='/status'>Status</a> | <a href='/ip'>IP</a></p>";
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/owm", []() {
        String html;
        html.reserve(512);
        html += "<!doctype html><html><head><meta charset='utf-8'>";
        html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
        html += "<title>OWM Status</title></head><body style='font-family:sans-serif'>";
        html += "<h1>OpenWeatherMap</h1>";
        html += "<p>API key source: hardcoded in config.h</p>";
        html += "<p>API key: ";
        html += String(Config::OWM_API_KEY).isEmpty() ? "not set" : "set";
        html += "</p><p>Location: ";
        html += Config::OWM_LOCATION;
        html += "</p>";
        if (owmClient != nullptr) {
            const WeatherData& weather = owmClient->getWeatherData();
            html += "<p>Temperature: ";
            html += String(weather.temperature, 1);
            html += " &deg;C</p><p>Humidity: ";
            html += String(weather.humidity, 0);
            html += " %</p><p>Pressure: ";
            html += String(weather.pressure, 0);
            html += " hPa</p><p>Description: ";
            html += weather.description.length() ? weather.description : "-";
            html += "</p><p>Last update: ";
            html += weather.timestamp > 0 ? String(weather.timestamp) : String("not fetched yet");
            html += "</p><p><a href='/owm/json'>Raw JSON</a></p>";
        } else {
            html += "<p>OWM client not attached.</p>";
        }
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/owm/json", []() {
        if (owmClient == nullptr) {
            server.send(503, "application/json", "{\"error\":\"OWM client not attached\"}");
            return;
        }
        server.send(200, "application/json", owmClient->getWeatherJson());
    });

    server.on("/ip", []() {
        server.send(200, "text/plain", WifiManager::isConnected() ? WifiManager::getLocalIP() : "not connected");
    });

    server.onNotFound([]() {
        server.send(404, "text/plain", "Not found");
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
}