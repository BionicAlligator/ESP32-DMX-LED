#include <dmx_read.h>
#include <led_strip_control.h>
#include <onboard_led_control.h>

#include <onboard_wifi.h>
#include <artnet_read.h>

u_int8_t dmx_data[512];
unsigned long lastUpdate = millis();

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
    u_int8_t r = dmx_data[i*3];
    u_int8_t g = dmx_data[i*3+1];
    u_int8_t b = dmx_data[i*3+2];
    leds[i].setRGB(r, g, b);
  }
}

void loop() {
    // Read from DMX the individual channel values, blocking, until timeout
    if (read_from_dmx(dmx_data)) {
        // IF DMX read successfully

        // Periodically log dmx received data
         if (millis() - lastUpdate > 100) {
            /* Print the received start code - it's usually 0. */
            Serial.printf("Start code is 0x%02X and slot %02X is 0x%02X\n", dmx_data[0], myDMXAddress, dmx_data[myDMXAddress]);
            lastUpdate = millis();
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
}