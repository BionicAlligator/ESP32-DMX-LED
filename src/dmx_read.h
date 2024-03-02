#pragma once

#include <esp_dmx.h>
#include <Arduino.h>

extern int myDMXAddress;
extern int transmitPin;
extern int receivePin;
extern int enablePin;
extern dmx_port_t dmxPort;

void dmx_setup();
int write_to_dmx(u_int8_t *);
int read_from_dmx(u_int8_t *);