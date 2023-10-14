#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <web_interface.h>

#define TEXT_BUFFER_SIZE 1024

AsyncWebServer server(80);
char text_buffer[TEXT_BUFFER_SIZE+1];
int next_writable_index=0;

void web_interface_append(const String& text) {
  /* Do not copy null termination character from text */
  uint16_t num_chars_without_null_terminator = text.length();

  /* Insure new text doesn't overflow text buffer */
  if (num_chars_without_null_terminator > TEXT_BUFFER_SIZE) {
    num_chars_without_null_terminator = TEXT_BUFFER_SIZE;
  }

  /* Move current buffer contents back to make enough space for new content */
  for (int i=TEXT_BUFFER_SIZE; i>=num_chars_without_null_terminator; i--) {
    text_buffer[i] = text_buffer[i-num_chars_without_null_terminator-1];
  }
  
  /* Separate new content with a newline */
  text_buffer[num_chars_without_null_terminator] = '\n';

  /* Copy new content in space previously created */
  for (int i=0; i<num_chars_without_null_terminator; i++) {
    text_buffer[i] = text.charAt(i);
  }
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void web_interface_setup() {
  /* Insure null termination of string */
  text_buffer[TEXT_BUFFER_SIZE] = '\0';

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", text_buffer);
  });

  server.onNotFound(notFound);

  server.begin();
}

