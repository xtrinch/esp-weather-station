#include "eeprom-handler.h"

void setupEEPROM() {
  EEPROM.begin(512);  //Initialize EEPROM
}

int getEEPROMAddress(const char * name) {
  if (strcmp(name, "ssid") == 0) {
    return WIFI_SSID_ADDRESS;
  } else if (strcmp(name, "password") == 0) {
    return WIFI_PASSWORD_ADDRESS;
  } else if (strcmp(name, "magic_string") == 0) {
    return MAGIC_STRING_ADDRESS;
  } else if (strcmp(name, "access_token") == 0) {
    return SENSOR_ACCESS_TOKEN_ADDRESS;
  }

  return -1;
}

bool readFromEEPROM(char * buf, const char * name) {
  int addr = getEEPROMAddress(name);
  if (addr < 0) {
    ardprintf("EEPROM: Could not read unknown value %s", name);
    return false;
  }

  for(int i=0; i < 60; i++) {
    char ch = (char)EEPROM.read(addr+i);
    buf[i] = ch;

    if (ch == '\0') {
      break;
    }
  }

  ardprintf("EEPROM: read %s: %s", name, buf);
  return true;
}

bool isConfigSaved() {
  #ifdef PRECONFIGURED
    return true;
  #endif
  
  char magicString[60];
  readFromEEPROM(magicString, "magic_string");

  if (strcmp(magicString, xstr(MAGIC_STRING)) != 0) {
    ardprintf("EEPROM: No WiFi credentials saved");
    return false;
  }

  return true;
}

bool clearConfig() {
  ardprintf("EEPROM: Clearing Wi-Fi credentials");
  return saveToEEPROM("magic_string", "");
}

bool saveConfig(const char * ssid, const char * password, const char * sensorAccessToken) {
  if (!ssid || strlen(ssid) == 0 || !password || strlen(password) == 0) {
    return false;
  }
  
  bool retVal = true;

  if (
    !saveToEEPROM("ssid", ssid) ||
    !saveToEEPROM("password", password) ||
    !saveToEEPROM("access_token", sensorAccessToken) ||
    !saveToEEPROM("magic_string", xstr(MAGIC_STRING))
  ) {
    retVal = false;
  }

  return retVal;
}

bool saveToEEPROM(const char * name, const char * value) { 
  int addr = getEEPROMAddress(name);

  if (addr < 0) {
    ardprintf("EEPROM: Unknown address for %s", name);
    return false;
  }
 
  for(int i=0; i < (int)strlen(value); i++) {
    EEPROM.write(addr + i, value[i]);
  }
  EEPROM.write(addr + strlen(value), '\0');

  bool success = EEPROM.commit(); 

  if (!success) {
    ardprintf("EEPROM: Could not save");
    return false;
  }

  ardprintf("EEPROM: Saved variable %s with value %s", name, value);
  return true;
}