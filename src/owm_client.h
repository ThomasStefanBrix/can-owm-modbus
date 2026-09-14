#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

struct WeatherData {
    float temperature = 0.0f;
    float feelsLike = 0.0f;
    float pressure = 0.0f;
    float humidity = 0.0f;
    float windSpeed = 0.0f;
    int cloudiness = 0;
    float sunshineHours = 0.0f;
    String description;
    String icon;
    unsigned long timestamp = 0;
};

class OWMClient {
public:
    OWMClient();

    void begin(const String& apiKey, const String& location);
    bool fetchWeather();

    const WeatherData& getWeatherData() const;
    float getSunshineHours(uint32_t intervalMs) const;
    String getWeatherJson() const;

private:
    struct SunshineSample {
        unsigned long timestamp;
        float sunshineHours;
    };

    static constexpr size_t HISTORY_SIZE = 96;

    String apiKey;
    String location;
    WeatherData lastWeather;
    SunshineSample sunshineHistory[HISTORY_SIZE] = {};
    size_t historyIndex = 0;

    String buildUrl() const;
    bool parseWeatherResponse(const String& payload);
    void recordSample(float sunshineHours);
};
