#ifndef ConfigManager_h
#define ConfigManager_h

#include <Arduino.h>
#include <CRC32.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

#include "Logger.h"

#define CONFIG_MEMORY_START 0
#define CONFIG_MEMORY_SIZE 256

struct WifiConfiguration {
  char ssid[32];
  char psk[64];
};

struct Configuration {
  char name[16];
  WifiConfiguration wifi;
};

class ConfigManager {
    public:
      Configuration config = {};

      ConfigManager();
      void load();
      void save();
      void reset();
    private:
      char * getDefaultName();
};
#endif