
#include <ArtnetWifi.h>
#include <Arduino.h>
#include <WiFi.h>
#include <local_wifi_password.h>

const char* _ssid = LOCAL_WIFI_SSID;
const char* _password = LOCAL_WIFI_PASSWORD;

void wifi_setup() {
  bool state = true;
  int i = 0;

  WiFi.begin(_ssid, _password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
}