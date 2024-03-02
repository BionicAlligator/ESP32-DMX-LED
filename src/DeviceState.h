#pragma once

class DeviceState
{
    int dmx_port1_artnet_universe = 1;

public:
    void setup();
    int get_dmx_port1_artnet_universe();
    void set_dmx_port1_artnet_universe(int new_value);
};

extern DeviceState device_state;

void device_state_setup();