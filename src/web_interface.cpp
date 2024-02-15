#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <StringArray.h>

#include <WiFiUdp.h>
#include <ArduinoMDNS.h>

#include <web_interface.h>
#include <spiffs_config.h>
#include <WebLog.h>
#include <SPIFFS.h>

#include <ArduinoJson.h>

WiFiUDP udp;
MDNS mdns(udp);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void send_websocket_update(AsyncWebSocketClient *client)
{
  // { "status": { "uptime_millis":1234 } }
  client->text("{ \"status\": { \"uptime_millis\":" + String(millis()) + " } }");
  Serial.printf("Sending update to websocket client %x\n", client);
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.println("Websocket client connection received");
    send_websocket_update(client);
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");
  }
  else if (type == WS_EVT_DATA) 
  {
    String s = String(data, len);
    Serial.println(s);

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.parseObject(s);

    const char* state = json["status"]["state"];
    Serial.println(state);
  }
}

long web_interface_last_update_attempt_millis = millis();

/* Must call this on main loop */
void web_interface_loop()
{
  mdns.run();

  if (millis() - web_interface_last_update_attempt_millis > 1000)
  {
    auto clients = ws.getClients();
    for(auto client = clients.begin(); client != clients.end(); ++client)
    {
      send_websocket_update(*client);
    }
    web_interface_last_update_attempt_millis = millis();
  }

  ws.cleanupClients();
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
  // Serve files in directory "/www/" when request url starts with "/"
  // Request to the root or none existing files will try to server the default
  // file name "index.htm" if exists
  server.serveStatic("/", SPIFFS, "/www/");
  server.onNotFound(web_request_not_found);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin();
}
