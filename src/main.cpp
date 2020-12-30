#include "main.h"

unsigned long displaySampleMillis = 1000000; // something large, so we surely execute refresh the first time around
#define DISPLAY_SAMPLE_INTERVAL (DISPLAY_REFRESH_RATE * mS_TO_S_FACTOR)

// U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R2, GPIO_NUM_18, GPIO_NUM_23, GPIO_NUM_2, GPIO_NUM_4, GPIO_NUM_22);
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup() {
  Serial.begin(9600);

  while (!Serial);

  if (!setupWiFi()) {
    goToSleep();
  }

  tft.begin();
}

void loop() {
  if (millis() - displaySampleMillis >= DISPLAY_SAMPLE_INTERVAL) {
    displaySampleMillis = millis();
    DynamicJsonDocument jsonMeasurements(1024);
    
    char url[150];
    snprintf(url, 500, "https://%s/api/measurements/display", CFG_SENSOR_DASHBOARD_URL);
    char accessToken[60] = CFG_ACCESS_TOKEN;
    char response[800]; // 648 
    makeSecureNetworkRequest(url, accessToken, "", response, "GET");
    DeserializationError error = deserializeJson(jsonMeasurements, response);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    
    serializeJsonPretty(jsonMeasurements, Serial);
    JsonObject documentRoot = jsonMeasurements.as<JsonObject>();

    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(0, 0);

    //tft.setFreeFont((const GFXfont *)LOAD_FONT4);
    ardprintf("Length %d", documentRoot.size());
    for (JsonPair keyValue : documentRoot) {
      JsonObject measurementRoot = keyValue.value();
      JsonObject measurementInfo = measurementRoot["info"];
      JsonObject measurements = measurementRoot["measurements"];

      // sensor name; e.g. living room sensor
      const char * displayName = measurementInfo["displayName"];
      tft.println(displayName);
      //ardprintf(displayName);

      int measurementTypeIndex = 0;
      for (JsonPair measurementKeyValue : measurements) {
        // measurement type; e.g. temperature
        // const char * measurementName = measurementKeyValue.key().c_str();
        const char * measurementName = measurementKeyValue.value()["measurementType"];
        //ardprintf(measurementName);
        //tft.println(measurementName);

        // measurement value; e.g. 26 (degrees)
        const char * measurementStr = measurementKeyValue.value()["measurement"];
        //ardprintf(measurementStr);
        //tft.println(measurementStr);

        // measurement unit; e.g. C
        const char * measurementUnit = measurementKeyValue.value()["measurementTypeUnit"];

        ardprintf("%s %s %s", measurementName, measurementStr, measurementUnit);
        tft.printf("%s %s %s \n", measurementName, measurementStr, measurementUnit);
        // ardprintf(measurementUnit);

        measurementTypeIndex += 1;
      }
      tft.println("");
    }

  }

  // delay(DISPLAY_REFRESH_RATE * mS_TO_S_FACTOR); // 5 min = 300000ms
}