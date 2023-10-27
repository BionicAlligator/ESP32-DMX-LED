#include <artnet_read.h>
#include <spiffs_config.h>
#include <WebLog.h>

// Set to true to print to log console DMX frames
const bool ARTNET_DMX_DEBUG = true;

ArtnetWifi _artnet;
int dmx_port1_artnet_universe = 1;

u_int8_t artnet_dmx_frames[ARTNET_MAX_UNIVERSES][ARTNET_MAX_CHANNELS_PER_UNIVERSE];

// DMX: Univ: 0, Seq: 249, Data (512): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
void artnet_dmx_debug(uint16_t artnet_universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
  bool tail = false;

  Log.print("artnet_read debug: artnet_universe:");
  Log.print(artnet_universe, DEC);
  Log.print(" sequence:");
  Log.print(sequence, DEC);
  Log.print(" length:");
  Log.print(length, DEC);
  Log.print(" data:");

  if (length > 32)
  {
    length = 32;
    tail = true;
  }

  for (int i = 0; i < length; i++)
  {
    Log.print(data[i], HEX);
    Log.print(" ");
  }
  if (tail)
  {
    Log.print("...");
  }
  Log.println();
}

void _onDmxFrame(uint16_t artnet_universe, uint16_t length, uint8_t sequence, uint8_t *data)
{

  if (artnet_universe >= 0 && artnet_universe < ARTNET_MAX_UNIVERSES)
  {
    uint16_t channels = (length <= ARTNET_MAX_CHANNELS_PER_UNIVERSE) ? length : ARTNET_MAX_CHANNELS_PER_UNIVERSE;

    for (u_int16_t c = 0; c < channels; c++)
    {
      artnet_dmx_frames[artnet_universe][c] = data[c];
    }
  }
  else
  {
    // Artnet frame is for a universe that we cannot store in our data structure
    // Ignore
  }

  if (ARTNET_DMX_DEBUG)
    artnet_dmx_debug(artnet_universe, length, sequence, data);
}

void artnet_setup()
{
  dmx_port1_artnet_universe = atoi(spiffs_config_get("wifi_manager_dmx_port1_artnet_universe").c_str());
  Log.printf("artnet_read: setting dmx_port1_artnet_universe to %d\n", dmx_port1_artnet_universe);
  _artnet.setArtDmxCallback(_onDmxFrame);
  _artnet.begin();
}

int read_from_artnet(u_int8_t *data)
{
  /* read() returns 0 if there are no new DMX frames.  If there are new frames, it calls our _onDmxFrame() callback but only once per read()*/
  if (_artnet.read() == ART_DMX)
  {
    /* Add in DMX Start Code because the data passed from Art-Net doesn't include it*/
    data[0] = 0;

    uint16_t channels = (ARTNET_MAX_CHANNELS_PER_UNIVERSE < 512) ? ARTNET_MAX_CHANNELS_PER_UNIVERSE : 512;

    for (u_int16_t c = 0; c < channels; c++)
    {
      data[c + 1] = artnet_dmx_frames[dmx_port1_artnet_universe][c];
    }

    return 1;
  }

  return 0;
}