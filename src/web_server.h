#pragma once

class OWMClient;

namespace WebServerApp {
void setOwmClient(OWMClient* client);
void begin();
void loop();
}