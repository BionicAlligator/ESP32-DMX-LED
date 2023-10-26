#pragma once

#include <WString.h>
#include <string>

String spiffs_config_get(String name);
void spiffs_config_set(String name, String value);
void spiffs_config_clear();