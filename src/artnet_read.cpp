#include <artnet_read.h>

// Set to true to print to serial console DMX frames
#define ARTNET_DMX_DEBUG true

ArtnetWifi _artnet;

u_int8_t artnet_dmx_frames[ARTNET_MAX_UNIVERSES][ARTNET_MAX_CHANNELS_PER_UNIVERSE];

// DMX: Univ: 0, Seq: 249, Data (512): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
void artnet_dmx_debug(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  bool tail = false;
  
  Serial.print("artnet_read debug: universe:");
  Serial.print(universe, DEC);
  Serial.print(" sequence:");
  Serial.print(sequence, DEC);
  Serial.print(" length:");
  Serial.print(length, DEC);
  Serial.print(" data:");
  
  if (length > 32) {
    length = 32;
    tail = true;
  }

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

void _onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  if (universe<ARTNET_MAX_UNIVERSES) {
    uint16_t channels = (length<ARTNET_MAX_CHANNELS_PER_UNIVERSE) ? length : ARTNET_MAX_CHANNELS_PER_UNIVERSE;
    for (u_int16_t c=0; c<channels; c++) {
      artnet_dmx_frames[universe][c] = data[c];
    }
  } else {
    // Artnet frame is for a universe that we cannot store in our data structure
    // Ignore
  }
  if (ARTNET_DMX_DEBUG) artnet_dmx_debug(universe, length, sequence, data);
}

void artnet_setup() {
  _artnet.setArtDmxCallback(_onDmxFrame);
  _artnet.begin();
}

int read_from_artnet(u_int8_t* data) {
  /* read() returns 0 if there are no new DMX frames.  If there are new frames, it calls our _onDmxFrame() callback but only once per read()*/
  if (_artnet.read()==ART_DMX) {
    uint16_t channels = (ARTNET_MAX_CHANNELS_PER_UNIVERSE<512) ? ARTNET_MAX_CHANNELS_PER_UNIVERSE:512;
    for (u_int16_t c=0; c<channels; c++) {
      data[c] = artnet_dmx_frames[0][c];
    }
  }
  return 0;
}