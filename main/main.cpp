#include <odroid_go.h>
#include "example.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define CHECK_ERROR_CODE(returned, expected) ({                        \
            if(returned != expected){                                  \
                abort();                                               \
            }                                                          \
})

namespace {
  const uint8_t TWDT_TIMEOUT_S = 3;
  static TaskHandle_t task_handles[portNUM_PROCESSORS];

  // We don't want to always update/init the speaker because it's noisy
  void odroid_go_begin(unsigned long baud = 115200) {
    // UART
    Serial.begin(baud);

    Serial.flush();
    Serial.print("ODROID_GO initializing...");

    //GO.Speaker.begin();

    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(BUTTON_MENU, INPUT_PULLUP);
    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_VOLUME, INPUT_PULLUP);
    pinMode(BUTTON_JOY_Y, INPUT_PULLDOWN);
    pinMode(BUTTON_JOY_X, INPUT_PULLDOWN);

    // ODROID_GO LCD INIT
    GO.lcd.begin();
    GO.lcd.setRotation(1);
    GO.lcd.fillScreen(BLACK);
    GO.lcd.setCursor(0, 0);
    GO.lcd.setTextColor(WHITE);
    GO.lcd.setTextSize(1);
    GO.lcd.setBrightness(255);

    // Battery
    GO.battery.begin();

    Serial.println("OK");
  }

  // Again, we don't want to always update/init the speaker because it's noisy
  void odroid_go_update() {
    //Button update
    GO.BtnA.read();
    GO.BtnB.read();
    GO.BtnMenu.read();
    GO.BtnVolume.read();
    GO.BtnSelect.read();
    GO.BtnStart.read();
    GO.JOY_Y.readAxis();
    GO.JOY_X.readAxis();

    //Speaker update
    //GO.Speaker.update();
    GO.battery.update();
  }

  // Resets state between examples
  void reset() {
    GO.lcd.setTextSize(1);
    GO.lcd.clear();
    GO.lcd.setCursor(0, 0);
    GO.lcd.setTextColor(WHITE);

    GO.Speaker.setVolume(0);
    GO.Speaker.end();
  }

  // Should be called when exiting, which is never
  void cleanup() {
    for(int i = 0; i < portNUM_PROCESSORS; i++){
      vTaskDelete(task_handles[i]);
      CHECK_ERROR_CODE(esp_task_wdt_delete(task_handles[i]), ESP_OK);
      CHECK_ERROR_CODE(esp_task_wdt_status(task_handles[i]), ESP_ERR_NOT_FOUND);

      CHECK_ERROR_CODE(esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(i)), ESP_OK);
      CHECK_ERROR_CODE(esp_task_wdt_status(xTaskGetIdleTaskHandleForCPU(i)), ESP_ERR_NOT_FOUND);
    }

    CHECK_ERROR_CODE(esp_task_wdt_deinit(), ESP_OK);
    CHECK_ERROR_CODE(esp_task_wdt_status(NULL), ESP_ERR_INVALID_STATE);
  }

  // Main loop and example initialization
  void loop(void *p) {
    (void)p;

    CHECK_ERROR_CODE(esp_task_wdt_add(NULL), ESP_OK);
    CHECK_ERROR_CODE(esp_task_wdt_status(NULL), ESP_OK);

    odroid_go_begin();

    Examples::make_example<Examples::HelloWorld>();
    Examples::make_example<Examples::Buttons>();
    Examples::make_example<Examples::BreathingLED>();
    Examples::make_example<Examples::Battery>();
    Examples::make_example<Examples::Speaker>();

    while (1) {
      if (esp_task_wdt_reset() != ESP_OK)
	break;

      odroid_go_update();

      if (Examples::current == nullptr || (GO.BtnMenu.wasPressed())) {
	reset();
	Examples::next_example();
      }
      else
	Examples::current->loop();

      taskYIELD();
    }

    cleanup();
  }
}

extern "C" void app_main() {
  CHECK_ERROR_CODE(esp_task_wdt_init(TWDT_TIMEOUT_S, false), ESP_OK);

#ifndef CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU0
  esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(0));
#endif
#ifndef CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU1
  esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(1));
#endif

  initArduino();

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  xTaskCreatePinnedToCore(&loop,		// Function
			  "main loop",		// Name
			  4096,			// Stack size
			  nullptr,		// Input
			  5,			// Priority
			  &task_handles[0],	// Handle
			  0);			// Core
}
