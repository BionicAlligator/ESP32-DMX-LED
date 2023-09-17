#include <Arduino.h>

int ledPin = 2;

void setLEDLevel(int value) {
  analogWrite(ledPin, value);
}