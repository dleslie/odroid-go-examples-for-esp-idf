#include <thread>
#include <mutex>

#include "example.h"
#include <odroid_go.h>
#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

class Examples::SpeakerImpl {
public:
  uint32_t readNotifications() {
    std::lock_guard<std::mutex> guard(lock);
    uint32_t n = notifications;
    notifications = 0;
    return n;
  }
  
  void writeNotification(uint32_t flag) {
    std::lock_guard<std::mutex> guard(lock);
    notifications |= flag;
  }

  std::mutex lock;
  uint32_t notifications = 0;
  TaskHandle_t task_handles[1];
};

namespace {
  enum Actions {
    Beep  = 1,
    Tone  = 2,
    Music = 4,
    Halt  = 8
  };
  
  void showStatus(const char *msg) {
    GO.lcd.clear();
    GO.lcd.setCursor(0, 0);
    GO.lcd.printf(msg);
  }

  void soundLoop(void *p) {
    Examples::SpeakerImpl *impl = (Examples::SpeakerImpl *)p;
    
    if (esp_task_wdt_add(NULL) != ESP_OK) {
      Serial.println("Sound Loop failed to add watch");
      return;
    }
    if (esp_task_wdt_status(NULL) != ESP_OK) {
      esp_task_wdt_delete(NULL);
      Serial.println("Sound Loop failed to get its own status");
      return;
    }

    Serial.println("Sound Loop entered");
    
    GO.Speaker.begin();
    GO.Speaker.setVolume(8);

    uint32_t notification = 0;
    while (true) {
      if (esp_task_wdt_reset() != ESP_OK) {
	Serial.println("Sound loop could not notify watch");
	break;
      }

      notification = impl->readNotifications();
      
      if (notification != 0)
	Serial.printf("Sound Loop notified %08x\n", notification);
   
      GO.Speaker.update();
      
      if (0 != (notification & Actions::Beep))
	GO.Speaker.beep();
      
      if (0 != (notification & Actions::Tone))
	GO.Speaker.tone(3000, 200);

      if (0 != (notification & Actions::Music))
	GO.Speaker.playMusic(m5stack_startup_music, 25000);

      if (0 != (notification & Actions::Halt))
	break;
      
      taskYIELD();
    }
    
    Serial.println("Sound Loop exitted");

    GO.Speaker.end();
    esp_task_wdt_delete(NULL);
    vTaskDelete(NULL);
  }
}

const char * Examples::Speaker::name() {
  return "Speaker";
}

void Examples::Speaker::setup() {
  if (!impl)
    impl = std::make_shared<Examples::SpeakerImpl>();
  
  xTaskCreatePinnedToCore(&soundLoop,       // Function
  			  "sound loop",	    // Name
  			  4096,	            // Stack size
  			  impl.get(),	    // Input
  			  10,		    // Priority
  			  &this->impl->task_handles[0], // Handle
  			  1);		    // Core

  GO.lcd.println("Speaker");
}

void Examples::Speaker::loop() {
  if(GO.BtnA.wasPressed()) {
    showStatus("wasPressed: A");
    impl->writeNotification(Actions::Beep);
  }
 
  if(GO.BtnB.wasPressed()) {
    showStatus("wasPressed: B");
    impl->writeNotification(Actions::Tone);
  }
 
  if(GO.BtnStart.wasPressed()) {
    showStatus("wasPressed: Start");
    impl->writeNotification(Actions::Music);
  }
}

void Examples::Speaker::takedown() {
  impl->writeNotification(Actions::Halt);
  
  while (esp_task_wdt_status(this->impl->task_handles[0]) == ESP_OK)
    delay(1);
}
