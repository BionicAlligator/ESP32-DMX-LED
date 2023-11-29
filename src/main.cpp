#include <dmx_read.h>
#include <led_strip_control.h>
#include <onboard_led_control.h>
#include <artnet_read.h>
#include <web_interface.h>
#include <wifi_manager.h>
#include <WebLog.h>

// Should we operate as a WiFi Art-Net node, reading Art-Net over WiFi and re-transmitting it as DMX?
// If false, we will act as a DMX receiver
const bool ARTNET_NODE = true;
const bool LED_STRIP_CONTROL = false;
const int MIN_DMX_REFRESH_RATE = 10;                       // Hz
const int MAX_UPDATE_PERIOD = 1000 / MIN_DMX_REFRESH_RATE; // milliseconds

u_int8_t dmx_data[512];
unsigned long lastUpdate = millis();

/* =-=-=-= BACKLOG OF WORK =-=-=-= */

// Sprint Goal: know we are ready to run the show (box created and tested with real lights, ideally with audience)
// DONE Tidy up code
// NOT OUR PROBLEM Configure Router and PC (SSID, Password, Physical Location)
// DONE Create case (final assembly)
// DONE MDNS
// DONE Web Interface
// DONE First Time Setup (Interactive WiFi Initialisation)
// DONE Web universe config
// DONE Test with actual lights
// DONE Test during school assemblies (with full audience)
// DONE Web DNS name config

// DONE Install 5V LED
// DONE Test DMX transmit and receive with second device
// DONE Web debug console (display log messages)
// DONE Complete physical build of second device
// DONE Unique WiFi AP name
// Create and freeze Release 1 code version

// Track down bug where sometimes the custom parameters entered on captive portal are not saved/applied (in this instance, node comes up on artnet.local)
// Add /universe/<new-artnet-universe> web endpoint to enable universe switching without needing a full reset
// Add /name/<new-node-name> web endpoint to enable node name (MDNS hostname) to be changed without needing a full reset
// Onboard LED control

// Rename DMX files
// Tidy code

// Table of DMX values on web page

// Fix orange wire on first device (used when receiving)
// Test second device as a receiver
// Web Mode config (RX/TX)
// Web LED strip control

// Second DMX port
// Document

// TO REVIEW...

/* Production build for v1.0.0
 - Build final firmware for upcoming deployment
 - Smoke test
*/

/* Graceful Artnet degradation
 - When Artnet updates are not occuring, do something sensible
 - When Wifi isn't available (or drops out), do something sensible
 - Fire test in wifi-congested areas
*/

/* Deployment: We Will Rock You!
 */

/* Full LED string support
 - Support the longest LED string that Vincent has (300 LED?)
*/

/* LED string mounting
 - Flexibly mount LED strings on stages
*/

/* Deployment: Battle of the bands
 */

void setup()
{
  Log.setup();

  // TODO: Slow blink LED = Starting Up

  wifi_manager_setup();

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