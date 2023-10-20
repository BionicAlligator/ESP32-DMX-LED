#include <SPIFFS.h> //this needs to be first, or it all crashes and burns...
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

#include <web_interface.h>
#include <spiffs_config.h>

DNSServer wifi_manager_dns;
char wifi_manager_universe[2];

AsyncWiFiManager wifi_manager(&server, &wifi_manager_dns);

// flag for saving data
bool shouldSaveConfig = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void wifi_manager_web_reset(AsyncWebServerRequest *request) {
  Serial.println("Resetting wifi manager settings");
  wifi_manager.resetSettings();
  Serial.println("Wifi manager settings reset");
  request->send(200, "text/plain", "Wifi manager settings reset.  Will restart");
  ESP.restart();
}

void register_reset_page_with_web_server() {
  Serial.println("Registering wifi manager reset at URL /reset");
  server.on("/reset", HTTP_ANY, wifi_manager_web_reset);
}

void spiffs_begin_reformatting_if_necessary() {
  !SPIFFS.begin(true);
}

void wifi_manager_setup()
{
  // read configuration from FS json and store in wifi_manager_universe
  spiffs_config_get("wifi_manager_universe").toCharArray(wifi_manager_universe, 3);

  AsyncWiFiManagerParameter wifi_manager_param_universe("universe", "Art-Net to DMX Universe", wifi_manager_universe, 2);
  wifi_manager.setSaveConfigCallback(saveConfigCallback);
  wifi_manager.addParameter(&wifi_manager_param_universe);

  // fetches ssid and pass and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  Serial.println("About to autoconnect...");

  if (!wifi_manager.autoConnect("Art-Net WiFi"))
  {
    Serial.println("Failed after autoconnect and hit timeout");
    delay(3000);
    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  Serial.print("Done with autoconnect...  Universe Selected: ");
  Serial.println(wifi_manager_param_universe.getValue());

  // save the custom parameters to FS
  if (shouldSaveConfig)
  {
    spiffs_config_set("wifi_manager_universe", wifi_manager_param_universe.getValue());
  }

  register_reset_page_with_web_server();
}
