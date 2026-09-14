#include "owm_client.h"

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

namespace {
unsigned long nowMs() {
    return millis();
}
}

OWMClient::OWMClient() = default;

void OWMClient::begin(const String& apiKeyValue, const String& locationValue) {
    apiKey = apiKeyValue;
    location = locationValue;
}

bool OWMClient::fetchWeather() {
    if (apiKey.isEmpty() || location.isEmpty()) {
        return false;
    }

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    if (!http.begin(client, buildUrl())) {
        return false;
    }

    const int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        http.end();
        return false;
    }

    const String payload = http.getString();
    http.end();
    return parseWeatherResponse(payload);
}

const WeatherData& OWMClient::getWeatherData() const {
    return lastWeather;
}

float OWMClient::getSunshineHours(uint32_t intervalMs) const {
    const unsigned long currentTime = nowMs();
    float sum = 0.0f;
    size_t count = 0;

    for (const auto& sample : sunshineHistory) {
        if (sample.timestamp != 0 && currentTime - sample.timestamp <= intervalMs) {
            sum += sample.sunshineHours;
            count++;
        }
    }

    return count > 0 ? sum / static_cast<float>(count) : 0.0f;
}

String OWMClient::getWeatherJson() const {
    JsonDocument doc;
    doc["temperature"] = lastWeather.temperature;
    doc["feels_like"] = lastWeather.feelsLike;
    doc["pressure"] = lastWeather.pressure;
    doc["humidity"] = lastWeather.humidity;
    doc["wind_speed"] = lastWeather.windSpeed;
    doc["cloudiness"] = lastWeather.cloudiness;
    doc["sunshine_hours"] = lastWeather.sunshineHours;
    doc["description"] = lastWeather.description;
    doc["icon"] = lastWeather.icon;
    doc["timestamp"] = lastWeather.timestamp;

    String result;
    serializeJson(doc, result);
    return result;
}

String OWMClient::buildUrl() const {
    return String("https://api.openweathermap.org/data/2.5/weather?q=") + location + "&appid=" + apiKey + "&units=metric&lang=de";
}

bool OWMClient::parseWeatherResponse(const String& payload) {
    JsonDocument doc;
    const DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        return false;
    }

    JsonObject main = doc["main"].as<JsonObject>();
    if (!main.isNull()) {
        lastWeather.temperature = main["temp"] | 0.0f;
        lastWeather.feelsLike = main["feels_like"] | 0.0f;
        lastWeather.pressure = main["pressure"] | 0.0f;
        lastWeather.humidity = main["humidity"] | 0.0f;
    }

    JsonObject wind = doc["wind"].as<JsonObject>();
    if (!wind.isNull()) {
        lastWeather.windSpeed = wind["speed"] | 0.0f;
    }

    JsonObject clouds = doc["clouds"].as<JsonObject>();
    if (!clouds.isNull()) {
        lastWeather.cloudiness = clouds["all"] | 0;
    }

    JsonArray weather = doc["weather"].as<JsonArray>();
    if (!weather.isNull() && weather.size() > 0) {
        JsonObject first = weather[0].as<JsonObject>();
        lastWeather.description = first["description"] | "";
        lastWeather.icon = first["icon"] | "";
    }

    lastWeather.timestamp = nowMs();
    lastWeather.sunshineHours = (100.0f - static_cast<float>(lastWeather.cloudiness)) / 100.0f * 12.0f;
    recordSample(lastWeather.sunshineHours);
    return true;
}

void OWMClient::recordSample(float sunshineHours) {
    sunshineHistory[historyIndex] = {nowMs(), sunshineHours};
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}
