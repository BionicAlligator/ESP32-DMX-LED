#include <SPIFFS.h> //this needs to be first, or it all crashes and burns...
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

AsyncWebServer wifi_manager_server(80);
DNSServer wifi_manager_dns;
char wifi_manager_universe[2];

// flag for saving data
bool shouldSaveConfig = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void wifi_manager_setup()
{
  // clean FS, for testing
  // SPIFFS.format();

  // read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin())
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json"))
    {
      // file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);

        if (json.success())
        {
          Serial.println("\nparsed json");

          strcpy(wifi_manager_universe, json["wifi_manager_universe"]);
        }
        else
        {
          Serial.println("failed to load json config");
        }
      }
    }
  }
  else
  {
    Serial.println("failed to mount FS");
  }
  // end read

  AsyncWiFiManager wifi_manager(&wifi_manager_server, &wifi_manager_dns);
  AsyncWiFiManagerParameter wifi_manager_param_universe("universe", "Art-Net to DMX Universe", wifi_manager_universe, 2);
  wifi_manager.setSaveConfigCallback(saveConfigCallback);
  wifi_manager.addParameter(&wifi_manager_param_universe);

  // reset settings - for testing
  // wifi_manager.resetSettings();

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
    Serial.println("Saving config");

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();

    strcpy(wifi_manager_universe, wifi_manager_param_universe.getValue());

    json["wifi_manager_universe"] = wifi_manager_universe;

    File configFile = SPIFFS.open("/config.json", "w");

    if (!configFile)
    {
      Serial.println("Failed to open config file for writing");
    }
    else
    {
      json.printTo(Serial);
      json.printTo(configFile);
      configFile.close();
    }
  }
}
