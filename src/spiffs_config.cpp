
#include <SPIFFS.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <WString.h>

void spiffs_config_begin_and_reformat_if_necessary()
{
  SPIFFS.begin(true);
}

JsonObject& spiffs_config_read() {
  Serial.println("mounting FS...");
  spiffs_config_begin_and_reformat_if_necessary();

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

        return json;
      }
      else
      {
        Serial.println("failed to load json config.  Using a new JSON object");
        DynamicJsonBuffer jsonBuffer;
        return jsonBuffer.createObject();
      }
    }
  }
}

String spiffs_config_get(String name)
{
  JsonObject &json = spiffs_config_read();

  if (&json==NULL) {
      return (String)NULL; // TODO something better than returning nulls
  } else {
    return json[name];
  }

}

void spiffs_config_set(String name, String value)
{
  Serial.println("Saving config");

  JsonObject &json = spiffs_config_read();

  if (&json==NULL) {
      return ;
  } else {
    json[name] = value;
    File configFile = SPIFFS.open("/config.json", "w");

    if (!configFile)
    {
      Serial.println("Failed to open config file for writing");
    }
    else
    {
      Serial.print("JSON config file: ");
      json.printTo(Serial);
      Serial.println();
      json.printTo(configFile);
      configFile.close();
    }
  }
}
