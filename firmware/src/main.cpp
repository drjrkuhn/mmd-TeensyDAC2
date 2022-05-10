#include "Arduino.h"

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop()
{
  digitalToggleFast(LED_BUILTIN);
  delay(250);
}
