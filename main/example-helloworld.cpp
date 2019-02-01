#include "example.h"
#include <odroid_go.h>

const char * Examples::HelloWorld::name() {
  return "Hello World";
}

void Examples::HelloWorld::setup() {
  GO.lcd.setTextSize(1);
  GO.lcd.print("Hello, ODROID-GO");
}
