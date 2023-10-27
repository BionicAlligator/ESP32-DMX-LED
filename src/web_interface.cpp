#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <WiFiUdp.h>
#include <ArduinoMDNS.h>

#include <web_interface.h>
#include <spiffs_config.h>
#include <WebLog.h>

WiFiUDP udp;
MDNS mdns(udp);


AsyncWebServer server(80);

/* Must call this on main loop */
void web_interface_loop()
{
  mdns.run();
}


void web_request_not_found(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void web_request_root(AsyncWebServerRequest *request)
{
  String s;
  Log.write_log_to(s);
  request->send(200, "text/plain", s); 
}

void web_interface_setup()
{
  /* Respond to MDNS queries for artnet.local, or configured hostname.local */
  String web_interface_param_mdns_hostname = spiffs_config_get("wifi_manager_mdns_hostname");

  if (&web_interface_param_mdns_hostname == NULL || web_interface_param_mdns_hostname.equals(""))
  {
    Log.println("Using MDNS hostname artnet.local");
    mdns.begin(WiFi.localIP(), "artnet");
  }
  else
  {
    Log.println("Using MDNS hostname " + web_interface_param_mdns_hostname);
    mdns.begin(WiFi.localIP(), web_interface_param_mdns_hostname.c_str());
  }

  server.on("/", HTTP_GET, web_request_root);

  server.onNotFound(web_request_not_found);

  server.begin();
}
