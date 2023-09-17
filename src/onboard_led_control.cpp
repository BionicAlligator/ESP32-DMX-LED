#include <Arduino.h>

int ledPin = 2;

void set_onboard_led_level(int value) {
  analogWrite(ledPin, value);
}