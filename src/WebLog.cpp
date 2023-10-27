
#include <Arduino.h>
#include <WebLog.h>

#define TEXT_BUFFER_SIZE 80 * 24
char text_buffer[TEXT_BUFFER_SIZE + 1];
int next_text_buffer_index = 0;

void WebLog::setup() {
  /* Start the serial connection back to the computer so that we can log
  messages to the Serial Monitor. Lets set the baud rate to 115200. */
  Serial.begin(115200);

  /* Insure null termination of string */
  text_buffer[TEXT_BUFFER_SIZE] = '\0';
}

size_t WebLog::write(uint8_t c)
{
  if (next_text_buffer_index < TEXT_BUFFER_SIZE) {
    text_buffer[next_text_buffer_index++] = c;
  } else {
    /* Shift characters 1 to the left */
    for (int i=0; i<TEXT_BUFFER_SIZE-1; i++) {
      text_buffer[i] = text_buffer[i+1];
    }
    /* Add new character at end */
    text_buffer[TEXT_BUFFER_SIZE-1] = c;
  }

  Serial.write(c);
  return 1;
}

void WebLog::write_log_to(String& write_to) {
  write_to += text_buffer;
}

WebLog Log;