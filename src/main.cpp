#include "main.h"

unsigned long displaySampleMillis = 1000000; // something large, so we surely execute refresh the first time around

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
GfxUi ui = GfxUi(&sprite); // Jpeg and bmpDraw functions

uint16_t width = 240;
uint16_t height = 320;
uint16_t edgeOffset = 5;

void setup() {
  SPIFFS.begin();
  Serial.begin(9600);
  while (!Serial);

  while (!setupWiFi());

  tft.begin();
  sprite.setColorDepth(8);
  sprite.createSprite(width, height);
  tft.fillRect(0, 0, width, height, TFT_BLACK);
}

void loop() {
  if (millis() - displaySampleMillis <= CFG_DISPLAY_SAMPLE_INTERVAL) {
    return;
  }
  displaySampleMillis = millis();

  // make iot freezer query
  char responseIotFreezer[800]; // 648 
  makeSecureNetworkRequest("https://iotfreezer.com/api/measurements/display", CFG_ACCESS_TOKEN, "", responseIotFreezer, "GET");

  // make open weather query
  char responseOpenWeather[800]; // 648 
  char url[250];
  snprintf(url, 250, "http://api.openweathermap.org/data/2.5/weather?q=%s&APPID=%s", CFG_OPENWEATHERMAP_LOCATION, CFG_OPENWEATHERMAP_APPID);
  makeNetworkRequest(url, "", "", responseOpenWeather, "GET");

  // deserialize open weather map
  DynamicJsonDocument jsonOpenWeather(1024);
  StaticJsonDocument<200> filter; // it contains "true" for each value we want to keep
  filter["weather"] = true;
  filter["dt"] = true; //unix date
  filter["timezone"] = true; //timezone, e.g. 3600
  filter["main"]["temp"] = true; // kelvin
  filter["main"]["humidity"] = true; // percent
  filter["main"]["feels_like"] = true; // kelvin
  filter["wind"] = true;
  filter["clouds"]["all"] = true;
  deserializeJson(jsonOpenWeather, responseOpenWeather, DeserializationOption::Filter(filter));
  
  // deserialize iot freezer
  DynamicJsonDocument jsonIotFreezer(1024);
  DeserializationError error = deserializeJson(jsonIotFreezer, responseIotFreezer);
  if (error) {
    ardprintf(error.c_str());
    return;
  }

  // clear screen
  sprite.fillRect(0, 0, width, height, TFT_BLACK);

  // current weather icon
  const char * weatherIcon =  jsonOpenWeather["weather"][0]["icon"];
  char weatherIconName [30];
  snprintf(weatherIconName, 30, "/icon/%s.bmp", weatherIcon);
  ui.drawBmp(weatherIconName, edgeOffset+25, 35);

  // current time
  long int date = jsonOpenWeather["dt"];
  long int timezoneOffset = jsonOpenWeather["timezone"];
  const long int timezonedDate = date + timezoneOffset;
  sprite.loadFont(AA_FONT_SMALL);
  sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
  sprite.drawString(asctime(gmtime(&timezonedDate)), edgeOffset + 25, edgeOffset);

  // current temperature
  ui.drawBmp("/icon/icon-temperature.bmp", 123, 40);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  double temperatureKelvin = jsonOpenWeather["main"]["temp"];
  temperatureKelvin -= 273.15;
  int temperatureCelcius = (int)round(temperatureKelvin);
  char temperature [10];
  snprintf(temperature, 10, "%*d", 2, temperatureCelcius);
  sprite.loadFont(AA_FONT_LARGE);
  sprite.drawString(temperature, 153, 40);
  sprite.loadFont(AA_FONT_SMALL);
  sprite.drawString("C\n", 198, 40);

  // temperature real feel
  sprite.loadFont(AA_FONT_SMALL);
  sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
  sprite.drawString("Real feel:", 130, 80);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  double feelsLikeKelvin = jsonOpenWeather["main"]["feels_like"];
  feelsLikeKelvin -= 273.15;
  int feelsLikeCelcius = (int)round(feelsLikeKelvin);
  char feelsLike [10];
  snprintf(feelsLike, 10, "%*d C", 2, feelsLikeCelcius);
  sprite.drawString(feelsLike, 130, 100);

  // current wind speed
  int windSpeed = (int)round(jsonOpenWeather["wind"]["speed"]);
  char weatherText [10];
  snprintf(weatherText, 10, "%*d", 2, windSpeed);
  sprite.loadFont(AA_FONT_LARGE);
  sprite.drawString(weatherText, edgeOffset + (width / 4) + 30, 135);
  sprite.loadFont(AA_FONT_SMALL);
  sprite.drawString("m/s", edgeOffset + (width / 4) + 75, 135);

  // wind direction
  int windDeg = jsonOpenWeather["wind"]["deg"];
  int windAngle = (windDeg + 22.5) / 45;
  if (windAngle > 7) windAngle = 0;
  char wind[][3] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW" };
  char windIconName [30];
  snprintf(windIconName, 30, "/wind/%s.bmp", wind[windAngle]);
  ui.drawBmp(windIconName, (width / 4), 135);

  // current humidity %
  sprite.loadFont(AA_FONT_LARGE);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  int humidity = jsonOpenWeather["main"]["humidity"];
  char humidityText [3];
  snprintf(humidityText, 3, "%d", humidity);
  sprite.drawString(humidityText, edgeOffset + 30, 185);
  sprite.loadFont(AA_FONT_SMALL);
  sprite.drawString("%", edgeOffset + 75, 185);
  ui.drawBmp( "/icon/icon-humidity.bmp", edgeOffset, 185);

  // current cloud %
  sprite.loadFont(AA_FONT_LARGE);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  int clouds = jsonOpenWeather["clouds"]["all"];
  char cloudsText [3];
  snprintf(cloudsText, 3, "%d", clouds);
  sprite.drawString(cloudsText, (width / 2) + edgeOffset + 30, 185);
  sprite.loadFont(AA_FONT_SMALL);
  sprite.drawString("%", (width / 2) + edgeOffset + 75, 185);
  ui.drawBmp( "/icon/icon-clouds.bmp", (width / 2), 185);

  // draw line between openweather and iotfreezer
  sprite.drawRect(0, 235, width, 1, TFT_LIGHTGREY);

  // draw iot freezer
  //serializeJsonPretty(json, Serial);
  JsonObject documentRoot = jsonIotFreezer.as<JsonObject>();

  sprite.loadFont(AA_FONT_SMALL);

  for (JsonPair keyValue : documentRoot) {
    JsonObject measurementRoot = keyValue.value();
    JsonObject measurements = measurementRoot["measurements"];

    // sensor name; e.g. living room sensor
    const char * displayName = measurementRoot["info"]["displayName"];
    const char * location = measurementRoot["info"]["location"];

    char name [50];
    snprintf(name, 50, "%s - %s", displayName, location);
    sprite.drawString(name, edgeOffset, 250);

    int idx = 0;
    for (JsonPair measurementKeyValue : measurements) {
      // measurement type; e.g. temperature
      const char * measurementName = measurementKeyValue.value()["measurementType"];
      // measurement value; e.g. 26 (degrees)
      const char * measurementStr = measurementKeyValue.value()["measurement"];
      // measurement unit; e.g. C
      const char * measurementUnit = measurementKeyValue.value()["measurementTypeUnit"];

      sprite.setTextColor(TFT_WHITE, TFT_BLACK);

      sprite.loadFont(AA_FONT_LARGE);
      sprite.drawString(measurementStr, edgeOffset + idx*120 + 30, 280);

      sprite.loadFont(AA_FONT_SMALL);
      sprite.drawString(measurementUnit, edgeOffset + idx*120 + 75, 280);

      // current weather icon
      char iconName [30];
      snprintf(iconName, 30, "/icon/icon-%s.bmp", measurementName);
      ui.drawBmp(iconName, edgeOffset + idx*120, 280);

      idx += 1;
    }
  }

  sprite.pushSprite(0, 0);
}