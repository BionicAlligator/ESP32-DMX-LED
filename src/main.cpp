#include <dmx_read.h>
#include <led_strip_control.h>
#include <onboard_led_control.h>

#include <onboard_wifi.h>
#include <artnet_read.h>

u_int8_t dmx_data[512];
unsigned long lastUpdate = millis();

/* =-=-=-= 2023-10-09 Sprint backlog.  Goal ESP32 Artnet Wifi node  =-=-=-= */

// DONE Andrew's local setup vscode (can flash esp32 from vscode)

// DONE Andrew's local setup for MagicQ

// DONE Board setup: ESP32 sender

// DONE Board setup: ESP32 reciever

// DONE New feature branch

// IN PROGRESS Code: Artnet to DMX

// Code: DMX to LED



/* =-=-=-= BACKLOG OF WORK =-=-=-= */

/* Artnet only
 - Drop DMX support for now (add back later, time permitting)
 - Architect main control loop for Artnet only
*/

/* Graceful Artnet degradation
 - When Artnet updates are not occuring, do something sensible
 - When Wifi isn't available (or drops out), do something sensible
 - Fire test in wifi-congested areas
*/

/* Full LED string support
 - Support the longest LED string that Vincent has (300 LED?)
*/

/* Production build for v1.0.0
 - Build final firmware for upcoming deployment
 - Smoke test
*/

/* LED string mounting
 - Flexibly mount LED strings on stages
*/

/* Deployment: Battle of the bands
*/

void setup() {
  /* Start the serial connection back to the computer so that we can log
    messages to the Serial Monitor. Lets set the baud rate to 115200. */
  Serial.begin(115200);

  wifi_setup();

  artnet_setup();

  dmx_setup();

  led_strip_setup();
}

void dmx_to_rgb(u_int8_t dmx_data[], CRGB leds[]) {
  for (int i=0; i<NUM_LEDS; i++) {
    u_int8_t r = dmx_data[i * 3 + LED_START_CHANNEL];
    u_int8_t g = dmx_data[i * 3 + LED_START_CHANNEL + 1];
    u_int8_t b = dmx_data[i * 3 + LED_START_CHANNEL + 2];
    leds[i].setRGB(r, g, b);
  }
}

void loop() {
  bool use_artnet = true; // Toggle between artnet and dmx

  if (use_artnet) {
    read_from_artnet(dmx_data);
    write_to_dmx(dmx_data);
  } else {
    // Read from DMX the individual channel values, blocking, until timeout
    if (read_from_dmx(dmx_data)) {
        // IF DMX read successfully

        // Periodically log dmx received data
         if (millis() - lastUpdate > 100) {
            /* Print the received start code - it's usually 0. */
            Serial.printf("Start code is 0x%02X and slot %02X is 0x%02X\n", dmx_data[0], myDMXAddress, dmx_data[myDMXAddress]);
            lastUpdate = millis();
        }
    }
  }

  set_onboard_led_level(dmx_data[myDMXAddress]);

  // if (dmx_data[myDMXAddress] >= 10 and dmx_data[myDMXAddress] <= 20) {
  //   Serial.println("Running white along strip...");
  //   // delay(1600);
  //   run_animation();
  // }

  // Convert from DMX channel data to combined RGB LED data
  dmx_to_rgb(dmx_data, leds);

  // Set LED strip based on combined RGB LED data
  led_strip_set();
}