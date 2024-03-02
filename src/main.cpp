#include <dmx_read.h>
#include <led_strip_control.h>
#include <onboard_led_control.h>
#include <artnet_read.h>
#include <web_interface.h>
#include <wifi_manager.h>
#include <DeviceState.h>
#include <WebLog.h>

// Should we operate as a WiFi Art-Net node, reading Art-Net over WiFi and re-transmitting it as DMX?
// If false, we will act as a DMX receiver
const bool ARTNET_NODE = true;
const bool LED_STRIP_CONTROL = false;
const int MIN_DMX_REFRESH_RATE = 10;                       // Hz
const int MAX_UPDATE_PERIOD = 1000 / MIN_DMX_REFRESH_RATE; // milliseconds

u_int8_t dmx_data[512];
unsigned long lastUpdate = millis();

/* =-=-=-= BACKLOG OF WORK =-=-=-=

DYNAMIC WEB INTERFACE
Move to a single dynamically updated web page that shows and allows updates to all parameters
Enable configuration to be changed and applied (or reset) without the need for a reboot

Status (read-only):
- Uptime
- IP address
- Signal Strength
- State (running, resetting, etc)

Configurable parameters (read-write):
- Hostname
- WiFi SSID & Password
- DMX Universe
- DMX Update Frequencies (max and min)
- DMX Channel Values
- DMX Receiver (e.g: LED strip driver) / ArtNet-to-DMX Gateway Mode

Angular app is integrated into the normal PIO build
Eliminate risk of someone injecting code within the hostname (or other config parameters) that they set from the captive portal & web interface
Track down bug where sometimes the custom parameters entered on captive portal are not saved/applied (in this instance, node comes up on artnet.local)

Graceful Artnet degradation
 - When Artnet updates are not occuring, do something sensible
 - When Wifi isn't available (or drops out), do something sensible
 - DONE Fire test in wifi-congested areas

Onboard LED control
Full LED string support
 - Support the longest LED string that Vincent has (300 LED?)
LED string mounting
 - Flexibly mount LED strings on stages
Second DMX port

Fix orange wire on first device (used when receiving)
Test second device as a DMX receiver (e.g: controlling LED strip)
Rename DMX files
Document

*/

void setup()
{
  Log.setup();

  // TODO: Slow blink LED = Starting Up

  wifi_manager_setup();

  device_state.setup();

  artnet_setup();

  dmx_setup();

  if (LED_STRIP_CONTROL)
    led_strip_setup();

  web_interface_setup();

  Log.println("Setup complete");
  Log.print(27, HEX);
  Log.println();
}

void loop()
{
  web_interface_loop();

  if (ARTNET_NODE)
  {
    if (read_from_artnet(dmx_data) || (millis() - lastUpdate > MAX_UPDATE_PERIOD))
    {
      write_to_dmx(dmx_data);
      lastUpdate = millis();
    }
  }
  else
  {
    // Read from DMX the individual channel values, blocking, until timeout
    if (read_from_dmx(dmx_data))
    {
      // IF DMX read successfully

      // Periodically log dmx received data
      if (millis() - lastUpdate > 100)
      {
        /* Print the received start code - it's usually 0. */
        Log.printf("Start code is 0x%02X and slot %02X is 0x%02X\n", dmx_data[0], myDMXAddress, dmx_data[myDMXAddress]);
        lastUpdate = millis();
      }
    }
  }

  // Status LED Modes:
  //   Factory Reset Mode
  //   No WiFi (doesn't apply in hardwired DMX mode)
  //   No Art-Net / No DMX (dependent on mode)
  //   No LED strip found
  // set_onboard_led_level(dmx_data[myDMXAddress]);

  if (LED_STRIP_CONTROL)
  {
    // if (dmx_data[myDMXAddress] >= 10 and dmx_data[myDMXAddress] <= 20) {
    //   Log.print("Running white along strip...");
    //   // delay(1600);
    //   run_animation();
    // }

    // Convert from DMX channel data to combined RGB LED data
    dmx_to_rgb(dmx_data, leds);

    // Set LED strip based on combined RGB LED data
    led_strip_set();
  }
}