#pragma once

#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;

void web_interface_setup();
void web_interface_append(const String&);
void web_interface_loop();