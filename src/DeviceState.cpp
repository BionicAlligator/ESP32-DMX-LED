#include <DeviceState.h>
#include <WebLog.h>
#include "spiffs_config.h"

int DeviceState::get_dmx_port1_artnet_universe()
{
    return dmx_port1_artnet_universe;
}

void DeviceState::set_dmx_port1_artnet_universe(int new_value)
{
    dmx_port1_artnet_universe = new_value;
    spiffs_config_set("wifi_manager_dmx_port1_artnet_universe", String(new_value));

    // TODO: When this value is set, we need to trigger an update to the front-end
}

void DeviceState::setup()
{
    dmx_port1_artnet_universe = atoi(spiffs_config_get("wifi_manager_dmx_port1_artnet_universe").c_str());
    Log.printf("device_state_setup: setting dmx_port1_artnet_universe to %d\n", dmx_port1_artnet_universe);
}

DeviceState device_state;
