#include <artnet_read.h>

ArtnetWifi _artnet;

void _onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  bool tail = false;
  
  Serial.print("DMX: Univ: ");
  Serial.print(universe, DEC);
  Serial.print(", Seq: ");
  Serial.print(sequence, DEC);
  Serial.print(", Data (");
  Serial.print(length, DEC);
  Serial.print("): ");
  
  if (length > 16) {
    length = 16;
    tail = true;
  }
  // send out the buffer
  for (int i = 0; i < length; i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  if (tail) {
    Serial.print("...");
  }
  Serial.println();
}

void artnet_setup() {
  _artnet.setArtDmxCallback(_onDmxFrame);
  _artnet.begin();
}

void artnet_read() {
  _artnet.read();
}