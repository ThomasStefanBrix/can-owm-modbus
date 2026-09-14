#include "app_config.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

#include "config.h"

namespace {
constexpr const char* CONFIG_PATH = "/app_config.json";
}

namespace AppConfig {
Data defaults() {
    Data data;
    data.owmApiKey = Config::OWM_API_KEY;
    data.owmLocation = Config::OWM_LOCATION;
    return data;
}

bool load(Data& data) {
    data = defaults();

    if (!LittleFS.exists(CONFIG_PATH)) {
        return false;
    }

    File file = LittleFS.open(CONFIG_PATH, "r");
    if (!file) {
        return false;
    }

    JsonDocument doc;
    const DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        return false;
    }

    data.owmApiKey = doc["owm_api_key"] | data.owmApiKey;
    data.owmLocation = doc["owm_location"] | data.owmLocation;
    return true;
}

bool save(const Data& data) {
    File file = LittleFS.open(CONFIG_PATH, "w");
    if (!file) {
        return false;
    }

    JsonDocument doc;
    doc["owm_api_key"] = data.owmApiKey;
    doc["owm_location"] = data.owmLocation;

    const bool ok = serializeJsonPretty(doc, file) > 0;
    file.close();
    return ok;
}
}