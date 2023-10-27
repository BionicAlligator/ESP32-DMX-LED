#pragma once

#include <Print.h>
#include <WString.h>

/* Writes to a buffer as well as Serial. */
class WebLog: public Print
{
  public:
    void setup();
    size_t write(uint8_t c);
    void write_log_to(String& write_to);
};

extern WebLog Log;