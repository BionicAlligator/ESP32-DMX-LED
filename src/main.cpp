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
// Web debug console (display log messages)
// Create and freeze Release 1 code version
// Complete physical build of second device

// Rename DMX files
// Tidy code

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
    if (read_from_artnet(dmx_data))
      write_to_dmx(dmx_data);
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
  //   No WiFi (doesn't apply in hardwired DMX mode)
  //   No Art-Net / No DMX (dependent on mode)
  //   No LED strip found
  set_onboard_led_level(dmx_data[myDMXAddress]);

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