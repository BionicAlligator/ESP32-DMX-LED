
#include <ArtnetWifi.h>
#include <Arduino.h>
#include <WiFi.h>
#include <local_wifi_password.h>

const char* ssid = LOCAL_WIFI_SSID;
const char* password = LOCAL_WIFI_PASSWORD;

void wifi_setup() {
  bool state = true;
  int i = 0;

  WiFi.begin(ssid, password);
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
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
}