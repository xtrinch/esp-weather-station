#include "sleep.h"

void goToSleep() {
  ardprintf("Sleep: Setup ESP32 to sleep for every %d seconds ", CFG_SLEEP_SECONDS);

  #ifdef ESP32
  esp_sleep_enable_timer_wakeup(CFG_SLEEP_SECONDS * uS_TO_S_FACTOR);
  // shut practically everything off
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
  #endif

  delay(1000);
  Serial.flush();

  #ifdef ESP32
  esp_deep_sleep_start();
  #elif ESP8266
  ESP.deepSleep(CFG_SLEEP_SECONDS * uS_TO_S_FACTOR, WAKE_RFCAL);
  #endif

  ardprintf("This will never be printed");
}