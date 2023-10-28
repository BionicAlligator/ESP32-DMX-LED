#include <SPIFFS.h> // This needs to be first, or it all crashes and burns...
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

#include <web_interface.h>
#include <spiffs_config.h>

#include <WebLog.h>

const int ARTNET_UNIVERSE_CHAR_LIMIT = 3;
const int MDNS_HOSTNAME_CHAR_LIMIT = 255;

DNSServer wifi_manager_dns;
char wifi_manager_dmx_port1_artnet_universe[ARTNET_UNIVERSE_CHAR_LIMIT];
char wifi_manager_mdns_hostname[MDNS_HOSTNAME_CHAR_LIMIT];

AsyncWiFiManager wifi_manager(&server, &wifi_manager_dns);

// Flag for saving data
bool shouldSaveConfig = false;

// Callback notifying us of the need to save config
void saveConfigCallback()
{
  Log.println("Should save config");
  shouldSaveConfig = true;
}

void wifi_manager_web_reset(AsyncWebServerRequest *request)
{
  Log.println("Resetting wifi manager settings");
  request->send(200, "text/plain", "Clearing WiFi and device config.  Search for \"Art-Net Wifi\" access point to perform first-time setup (may take around 30 seconds to appear).  CLOSE YOUR BROWSER NOW.");
  spiffs_config_clear();
  // Delay required to ensure HTTP response has time to get back to browser before we cut the WiFi connection.
  delay(1000);
  wifi_manager.resetSettings();
  Log.println("Wifi manager settings reset");
  // Delay required to ensure WiFi settings are reset before rebooting the device.
  delay(1000);
  ESP.restart();
}

void register_reset_page_with_web_server()
{
  Log.println("Registering wifi manager reset at URL /reset");
  server.on("/reset", HTTP_ANY, wifi_manager_web_reset);
}

void spiffs_begin_reformatting_if_necessary()
{
  !SPIFFS.begin(true);
}

void wifi_manager_setup()
{
  // Read configuration from JSON file on filesystem
  spiffs_config_get("wifi_manager_dmx_port1_artnet_universe").toCharArray(wifi_manager_dmx_port1_artnet_universe, ARTNET_UNIVERSE_CHAR_LIMIT);
  spiffs_config_get("wifi_manager_mdns_hostname").toCharArray(wifi_manager_mdns_hostname, MDNS_HOSTNAME_CHAR_LIMIT);

  AsyncWiFiManagerParameter wifi_manager_param_dmx_port1_artnet_universe("universe", "DMX Port 1 Art-Net Universe", wifi_manager_dmx_port1_artnet_universe, ARTNET_UNIVERSE_CHAR_LIMIT);
  AsyncWiFiManagerParameter wifi_manager_param_mdns_hostname("mdns_hostname", "Node Name", wifi_manager_mdns_hostname, MDNS_HOSTNAME_CHAR_LIMIT);
  wifi_manager.setSaveConfigCallback(saveConfigCallback);
  wifi_manager.addParameter(&wifi_manager_param_dmx_port1_artnet_universe);
  wifi_manager.addParameter(&wifi_manager_param_mdns_hostname);

  // Fetches SSID and password and tries to connect
  // If it fails to connect, it starts an access point with the specified SSID
  Log.println("About to autoconnect...");

  String ssid = "Art-Net WiFi " + WiFi.macAddress().substring(9);

  // TODO: Fast blink LED = WiFi not connected or first-time config (AP mode)

  if (!wifi_manager.autoConnect(ssid.c_str()))
  {
    Log.println("Failed after autoconnect and hit timeout");
    delay(3000);
    // Reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  // TODO: Turn off LED - we are on WiFi

  Log.print("Done with autoconnect...  Universe Selected: ");
  Log.println(wifi_manager_param_dmx_port1_artnet_universe.getValue());
  Log.println();
  Log.print("MDNS Hostname:");
  Log.println(wifi_manager_param_mdns_hostname.getValue());
  Log.println();

  // Save the custom parameters to filesystem
  if (shouldSaveConfig)
  {
    spiffs_config_set("wifi_manager_dmx_port1_artnet_universe", wifi_manager_param_dmx_port1_artnet_universe.getValue());
    spiffs_config_set("wifi_manager_mdns_hostname", wifi_manager_param_mdns_hostname.getValue());
  }

  register_reset_page_with_web_server();
}
