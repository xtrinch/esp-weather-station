#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "common.h"
#include "station.h"
#include "GfxUi.h"
#include "SPIFFS.h"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>

#define CFG_DISPLAY_SAMPLE_INTERVAL (DISPLAY_REFRESH_RATE * mS_TO_S_FACTOR)
#define CFG_OPENWEATHERMAP_LOCATION xstr(OPENWEATHERMAP_LOCATION)
#define CFG_OPENWEATHERMAP_APPID xstr(OPENWEATHERMAP_APPID)

#define AA_FONT_SMALL "fonts/NotoSansBold15" // 15 point sans serif bold
#define AA_FONT_LARGE "fonts/NotoSansBold36" // 36 point sans serif bold

#endif