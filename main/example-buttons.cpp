#include "example.h"
#include <odroid_go.h>
#include <Arduino.h>

const char * Examples::Buttons::name() {
  return "Buttons";
}

void Examples::Buttons::setup() {
  GO.lcd.setTextSize(2);
}

void Examples::Buttons::loop() {
  if (GO.JOY_Y.wasReleased()
      || GO.JOY_X.wasReleased()
      || GO.BtnVolume.wasReleased()
      || GO.BtnStart.wasReleased()
      || GO.BtnSelect.wasReleased()
      || GO.BtnA.wasReleased()
      || GO.BtnB.wasReleased())
    GO.lcd.clear();

  GO.lcd.setCursor(0, 0);

  GO.lcd.setTextColor(WHITE);
  GO.lcd.println("Direction pad");
  
  GO.lcd.setTextColor(YELLOW);
  GO.lcd.printf("Joy-Y-Up: %s \n", (GO.JOY_Y.isAxisPressed() == 2) ? "Pressed" : "");
  GO.lcd.printf("Joy-Y-Down: %s \n", (GO.JOY_Y.isAxisPressed() == 1) ? "Pressed" : "");
  GO.lcd.printf("Joy-X-Left: %s \n", (GO.JOY_X.isAxisPressed() == 2) ? "Pressed" : "");
  GO.lcd.printf("Joy-X-Right: %s \n", (GO.JOY_X.isAxisPressed() == 1) ? "Pressed" : "");
  GO.lcd.println("");
  
  GO.lcd.setTextColor(WHITE);
  GO.lcd.println("Function key");
 
  GO.lcd.setTextColor(YELLOW);
  GO.lcd.printf("Volume: %s \n", (GO.BtnVolume.isPressed() == 1) ? "Pressed" : "");
  GO.lcd.printf("Select: %s \n", (GO.BtnSelect.isPressed() == 1) ? "Pressed" : "");
  GO.lcd.printf("Start: %s \n", (GO.BtnStart.isPressed() == 1) ? "Pressed" : "");
  GO.lcd.println("");
  
  GO.lcd.setTextColor(WHITE);
  GO.lcd.println("Actions");
  
  GO.lcd.setTextColor(YELLOW);
  GO.lcd.printf("B: %s \n", (GO.BtnB.isPressed() == 1) ? "Pressed" : "");
  GO.lcd.printf("A: %s \n", (GO.BtnA.isPressed() == 1) ? "Pressed" : "");
}
