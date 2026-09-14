#pragma once

#include <Arduino.h>

namespace AppConfig {
struct Data {
    String owmApiKey;
    String owmLocation;
};

bool load(Data& data);
bool save(const Data& data);
Data defaults();
}