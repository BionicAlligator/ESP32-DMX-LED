#include <artnet_read.h>

ArtnetWifi artnet;

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  Serial.println("Hello Artnet world!  got a dmx frame");
}

void artnet_setup() {
  artnet.begin();
  artnet.setArtDmxCallback(onDmxFrame);
}