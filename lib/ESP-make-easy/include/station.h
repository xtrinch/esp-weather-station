#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

#include <Arduino.h>
#ifdef ESP32
  #include <HTTPClient.h>
  #include <WiFi.h>
#elif ESP8266
  #include <ESP8266HTTPClient.h>
  #include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include "common.h"
#include "eeprom-handler.h"

#define WIFI_CONNECT_RETRIES 50

bool setupWiFi();
bool makeNetworkRequest(const char * url, const char * authorization, const char * content, const char * response, const char * method);
bool makeSecureNetworkRequest(const char * url, const char * authorization, const char * content, const char * response, const char * method);

#endif