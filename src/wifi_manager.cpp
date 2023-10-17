
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

AsyncWebServer wifi_manager_server(80);
DNSServer wifi_manager_dns;

void wifi_manager_setup() {
  AsyncWiFiManager wifi_manager(&wifi_manager_server,&wifi_manager_dns);
  wifi_manager.autoConnect("ArtNet Wifi");
}

