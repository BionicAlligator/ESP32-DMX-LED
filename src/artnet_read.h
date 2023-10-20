#pragma once
#include <ArtnetWifi.h>

#define ARTNET_MAX_CHANNELS_PER_UNIVERSE 512
#define ARTNET_MAX_UNIVERSES 18

void artnet_setup();
int read_from_artnet(u_int8_t*);