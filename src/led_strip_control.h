
#include <FastLED.h>

#define NUM_LEDS 150
#define LED_START_CHANNEL 1

extern CRGB leds[];

void led_strip_set();
void led_strip_setup();
void dmx_to_rgb(u_int8_t dmx_data[], CRGB leds[]);
void run_animation();
