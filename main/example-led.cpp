#include "example.h"
#include <odroid_go.h>
#include <Arduino.h>

#define PIN_BLUE_LED  2
#define PWM_CHANNEL   1

namespace {
  unsigned long last_tick = 0;
}

const char * Examples::BreathingLED::name() {
  return "Breathing LED";
}

void Examples::BreathingLED::setup() {
  pinMode(PIN_BLUE_LED, OUTPUT);
  digitalWrite(PIN_BLUE_LED, HIGH);

  ledcAttachPin(PIN_BLUE_LED, PWM_CHANNEL);
  ledcSetup(PWM_CHANNEL, 12000, 8);

  pinVal = 0;
  ascending = true;

  GO.lcd.println("LED");
}

void Examples::BreathingLED::loop() {
  unsigned long now = millis();
  if (last_tick - now < 3)
    return;
  last_tick = now;
  
  if (ascending) {
    pinVal = pinVal + 1;
    if (pinVal >= 254)
      ascending = false;
  }
  else {
    pinVal = pinVal - 1;
    if (pinVal <= 0)
      ascending = true;
  }
  
  ledcWrite(PWM_CHANNEL, (uint8_t)pinVal);
}

void Examples::BreathingLED::takedown() {
  ledcWrite(PWM_CHANNEL, 255);
}
