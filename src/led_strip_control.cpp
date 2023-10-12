#include <Arduino.h>
#include <FastLED.h>
#include <led_strip_control.h>

#define DATA_PIN 22
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

void led_strip_set()
{
  FastLED.show();
}

// This function sets up the ledsand tells the controller about them
void led_strip_setup()
{
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

void dmx_to_rgb(u_int8_t dmx_data[], CRGB leds[])
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    u_int8_t r = dmx_data[i * 3 + LED_START_CHANNEL];
    u_int8_t g = dmx_data[i * 3 + LED_START_CHANNEL + 1];
    u_int8_t b = dmx_data[i * 3 + LED_START_CHANNEL + 2];
    leds[i].setRGB(r, g, b);
  }
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void run_animation()
{
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
  {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(100);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;

    FastLED.show();
  }

  for (int whiteLed = NUM_LEDS - 1; whiteLed >= 0; whiteLed = whiteLed - 1)
  {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(100);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;

    FastLED.show();
  }
}