#include "ping.h"

unsigned long lastPingMillis = 0;

bool sendPing() {
  char url[150];
  snprintf(url, 500, "https://%s/api/forwarders/ping", CFG_SENSOR_DASHBOARD_URL);
  return makeSecureNetworkRequest(url, CFG_ACCESS_TOKEN, "", NULL, "POST");
}

bool checkIfShouldPing() {
  if (millis() - lastPingMillis >= PING_INTERVAL * 1000UL) {
    lastPingMillis = millis(); 
    sendPing();
    ardprintf("Pinged server.");
    return true;
  }

  return false;
}