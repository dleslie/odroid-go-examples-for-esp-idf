#include "example.h"
#include <odroid_go.h>
#include <Arduino.h>

#include <driver/adc.h>
#include <esp_adc_cal.h>

#define RESISTANCE_NUM    2
#define DEFAULT_VREF      1100

namespace {
  unsigned long last_tick = 0;
  
  static esp_adc_cal_characteristics_t adc_chars;
}

const char * Examples::Battery::name() {
  return "Battery";
}

void Examples::Battery::setup() {
  GO.lcd.setTextSize(2);
 
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);
}

double readBatteryVoltage() {
  return (double) esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc_chars) * RESISTANCE_NUM / 1000;
}
 
void showBatteryVoltage(double voltage) {
  GO.lcd.clear();
  GO.lcd.setCursor(0, 0);
  
  GO.lcd.printf("Current Voltage: %1.3lf V \n", voltage);
}

void Examples::Battery::loop() {
  unsigned long now = millis();
  if (now - last_tick >= 1000) {
    last_tick = now;
    showBatteryVoltage(readBatteryVoltage());
  }
}
