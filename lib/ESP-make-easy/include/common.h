#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <string.h>
#include <stdarg.h>
#include <Arduino.h>

#define ARDBUFFER 32

// macro to string expansion for env variables
#define xstr(s) strs(s)
#define strs(s) #s

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR 1000  /* Conversion factor for mili seconds to seconds */

#define CFG_ONBOARD_WIFI_SSID xstr(ONBOARD_WIFI_SSID)
#define CFG_ONBOARD_WIFI_PASSWORD xstr(ONBOARD_WIFI_PASSWORD)
#define CFG_SENSOR_DASHBOARD_URL xstr(SENSOR_DASHBOARD_URL)
#define CFG_SLEEP_SECONDS atoi(xstr(SLEEP_SECONDS))
#define CFG_WIFI_SSID xstr(WIFI_SSID)
#define CFG_WIFI_PASSWORD xstr(WIFI_PASSWORD)
#define CFG_PING_URL xstr(PING_URL)
#define CFG_ACCESS_TOKEN xstr(ACCESS_TOKEN)
#define CFG_CERTIFICATE xstr(CERTIFICATE)
extern const uint8_t certificate_start[] asm("_binary_certificate_cer_start");
extern const uint8_t certificate_end[] asm("_binary_certificate_cer_end");

void ardprintf(const char *str, ...);
void append(char* s, char c);

#endif