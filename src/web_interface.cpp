#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <WiFiUdp.h>
#include <ArduinoMDNS.h>

#include <web_interface.h>
#include <spiffs_config.h>
#include <WebLog.h>
#include <SPIFFS.h>

WiFiUDP udp;
MDNS mdns(udp);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

AsyncWebSocketClient *myClient;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.println("Websocket client connection received");
    client->text("{\"millis\":123}");

    myClient = client;
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");
    myClient = NULL;
  }
}

long webLastUpdate = millis();

/* Must call this on main loop */
void web_interface_loop()
{
  mdns.run();

  if ((millis() - webLastUpdate > 1000) && (myClient != NULL))
  {
    Serial.println("Updating Websocket value");
    myClient->text("{\"millis\":" + String(millis()) + "}");

    webLastUpdate = millis();
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

const int MAX_TEMPLATE_VARIABLES = 10;
String *template_variable_names[MAX_TEMPLATE_VARIABLES];
String *template_variable_values[MAX_TEMPLATE_VARIABLES];
int next_free_slot = 0;

void register_template_variable(const char *name, const char *value)
{
  if (next_free_slot < MAX_TEMPLATE_VARIABLES)
  {
    template_variable_names[next_free_slot] = new String(name);
    template_variable_values[next_free_slot] = new String(value);
    next_free_slot++;
  }
  else
  {
    Log.println("Maximum template variables reached. Can not register additional variable");
  }
}

String templateProcessor(const String &var)
{
  if (var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");

  for (int variable_index = 0; variable_index < next_free_slot; variable_index++)
  {
    if (var == *template_variable_names[variable_index])
    {
      return F(template_variable_values[variable_index]);
    }
  }

  return String();
}

void web_interface_setup()
{
  /* Respond to MDNS queries for artnet.local, or configured hostname.local */
  String web_interface_param_mdns_hostname = spiffs_config_get("wifi_manager_mdns_hostname");

  if (&web_interface_param_mdns_hostname == NULL || web_interface_param_mdns_hostname.equals(""))
  {
    Log.println("Using MDNS hostname artnet.local");
    mdns.begin(WiFi.localIP(), "artnet");
    register_template_variable("WEBSOCKET_URL", "ws://artnet.local:80/ws");
  }
  else
  {
    Log.println("Using MDNS hostname " + web_interface_param_mdns_hostname);
    mdns.begin(WiFi.localIP(), web_interface_param_mdns_hostname.c_str());
    register_template_variable("WEBSOCKET_URL", ("ws://" + web_interface_param_mdns_hostname + ".local:80/ws").c_str());
  }

  server.on("/", HTTP_GET, web_request_root);
  // Serve files in directory "/www/" when request url starts with "/"
  // Request to the root or none existing files will try to server the default
  // file name "index.htm" if exists
  server.serveStatic("/", SPIFFS, "/www/"); //.setTemplateProcessor(templateProcessor);
  server.onNotFound(web_request_not_found);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin();
}
