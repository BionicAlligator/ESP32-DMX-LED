
#include <SPIFFS.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <WString.h>
#include <WebLog.h>

void spiffs_config_begin_and_reformat_if_necessary()
{
  SPIFFS.begin(true);
}

JsonObject& spiffs_config_read() {
  Log.println("mounting FS...");
  spiffs_config_begin_and_reformat_if_necessary();

  Log.println("mounted file system");
  if (SPIFFS.exists("/config.json"))
  {
    // file exists, reading and loading
    Log.println("reading config file");
    File configFile = SPIFFS.open("/config.json", "r");
    if (configFile)
    {
      Log.println("opened config file");
      size_t size = configFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      configFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject &json = jsonBuffer.parseObject(buf.get());
      json.printTo(Log);

      if (json.success())
      {
        Log.println("\nparsed json");

        return json;
      }
      else
      {
        Log.println("failed to load json config.  Using a new JSON object");
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
  Log.print("Saving config");

  JsonObject &json = spiffs_config_read();

  if (&json==NULL) {
      return ;
  } else {
    json[name] = value;
    File configFile = SPIFFS.open("/config.json", "w");

    if (!configFile)
    {
      Log.print("Failed to open config file for writing");
    }
    else
    {
      Log.print("JSON config file: ");
      json.printTo(Log);
      json.printTo(configFile);
      configFile.close();
    }
  }
}

void spiffs_config_clear() {
  SPIFFS.remove("/config.json");
}