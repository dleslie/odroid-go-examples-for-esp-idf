#include "example.h"
#include <odroid_go.h>
#include <Arduino.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED)
#  error "Requires CONFIG_BT_ENABLED"
#endif

#if !defined(CONFIG_BLUEDROID_ENABLED)
#  error "Requires CONFIG_BLUEDROID_ENABLED"
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#  error "Requires CONFIG_BT_SPP_ENABLED"
#endif

class Examples::BluetoothImpl {
public:
  BluetoothSerial serialBT;

  void writeLineToBT(const char *data) {
    for (const uint8_t *ptr = (const uint8_t *)data; *ptr != 0; ++ptr)
      serialBT.write(*ptr);
  }
};

const char * Examples::Bluetooth::name() {
  return "Bluetooth Serial";
}

void Examples::Bluetooth::setup() {
  if (!impl)
    impl = std::make_shared<Examples::BluetoothImpl>();

  impl->serialBT.begin("ODROID-GO");
  
  // if (esp_bluedroid_enable() != ESP_OK) {
  //   Serial.println("Failed to start BT");
  //   return;
  // }

  const uint8_t *address = esp_bt_dev_get_address();
  if (address == nullptr) {
    Serial.println("Could not read BT address");
    return;
  }
  Serial.printf("BT address: %x%x%x%x%x%x\n",
		address[0], address[1], address[2],
		address[3], address[4], address[5]);
  
  // if (esp_bt_dev_set_device_name("ODROID-GO") != ESP_OK)
  //   Serial.println("Could not set device name");
  
  GO.lcd.println("Bluetooth");
}

void Examples::Bluetooth::loop() {
  if (impl->serialBT.available())
    GO.lcd.println(impl->serialBT.read());
  if (GO.BtnA.wasPressed() == 1)
    impl->writeLineToBT("Button A pressed\n");
  if (GO.BtnB.wasPressed() == 1)
    impl->writeLineToBT("Button B pressed\n");
}

void Examples::Bluetooth::takedown() {
  impl->serialBT.end();
}
