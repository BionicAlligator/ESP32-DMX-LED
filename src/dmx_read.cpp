/*

  DMX Read

  This sketch allows you to read DMX from a DMX controller using a standard DMX
  shield, such SparkFun ESP32 Thing Plus DMX to LED Shield. This sketch was
  made for the Arduino framework!

  Created 9 September 2021
  By Mitch Weisbrod

  https://github.com/someweisguy/esp_dmx

*/
#include <Arduino.h>
#include <esp_dmx.h>

int myDMXAddress = 9;

/* First, lets define the hardware pins that we are using with our ESP32. We
  need to define which pin is transmitting data and which pin is receiving data.
  DMX circuits also often need to be told when we are transmitting and when we
  are receiving data. We can do this by defining an enable pin. */
int transmitPin = 17;
int receivePin = 16;
int enablePin = 21;
/* Make sure to double-check that these pins are compatible with your ESP32!
  Some ESP32s, such as the ESP32-WROVER series, do not allow you to read or
  write data on pins 16 or 17, so it's always good to read the manuals. */

/* Next, lets decide which DMX port to use. The ESP32 has either 2 or 3 ports.
  Port 0 is typically used to transmit serial data back to your Serial Monitor,
  so we shouldn't use that port. Lets use port 1! */
dmx_port_t dmxPort = 1;

void dmx_setup() {
  /* Now we will install the DMX driver! We'll tell it which DMX port to use, 
    what device configure to use, and which interrupt priority it should have. 
    If you aren't sure which configuration or interrupt priority to use, you can
    use the macros `DMX_CONFIG_DEFAULT` and `DMX_INTR_FLAGS_DEFAULT` to set the
    configuration and interrupt to their default settings. */
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_driver_install(dmxPort, &config, DMX_INTR_FLAGS_DEFAULT);

  /* Now set the DMX hardware pins to the pins that we want to use and setup
    will be complete! */
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
}

int read_from_dmx(u_int8_t* data) {
  /* We need a place to store information about the DMX packets we receive. We
    will use a dmx_packet_t to store that packet information.  */
  dmx_packet_t packet;

  /* And now we wait! The DMX standard defines the amount of time until DMX
    officially times out. That amount of time is converted into ESP32 clock
    ticks using the constant `DMX_TIMEOUT_TICK`. If it takes longer than that
    amount of time to receive data, this if statement will evaluate to false. */
   if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
        /* Get the current time since boot in milliseconds so that we can find out
      how long it has been since we last updated data and printed to the Serial
      Monitor. */
    unsigned long now = millis();

    /* We should check to make sure that there weren't any DMX errors. */
    if (!packet.err) {
        /* Don't forget we need to actually read the DMX data into our buffer so
        that we can print it out. */
      dmx_read(dmxPort, data, packet.size);
      return 1;
    } else {
        printf("Packet recieved, but with error\n");
        return 0;
    }
   } else {
      /* Oops! A DMX error occurred! Don't worry, this can happen when you first
        connect or disconnect your DMX devices. If you are consistently getting
        DMX errors, then something may have gone wrong with your code or
        something is seriously wrong with your DMX transmitter. */
      Serial.println("A DMX error occurred.");

      switch (packet.err) {
        case DMX_OK:
          printf("Received packet with start code: %02X and size: %i.\n",
            packet.sc, packet.size);
          // Data is OK. Now read the packet into the buffer.
          dmx_read(DMX_NUM_2, data, packet.size);
          break;
        
        case DMX_ERR_TIMEOUT:
          printf("The driver timed out waiting for the packet.\n");
          /* If the provided timeout was less than DMX_TIMEOUT_TICK, it may be
            worthwhile to call dmx_receive() again to see if the packet could be
            received. */
          break;

        case DMX_ERR_IMPROPER_SLOT:
          printf("Received malformed byte at slot %i.\n", packet.size);
          /* A slot in the packet is malformed. Data can be recovered up until 
            packet.size. */
          break;

        case DMX_ERR_UART_OVERFLOW:
          printf("The DMX port overflowed.\n");
          /* The ESP32 UART overflowed. This could occur if the DMX ISR is being
            constantly preempted. */
          break;
      }
      return 0;
    }
}