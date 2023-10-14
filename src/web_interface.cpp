#include <web_interface.h>

AsyncWebServer server(80);

void web_interface_setup() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    server.begin();
}