#include "ConfigManager.h"

ConfigManager::ConfigManager() {
    // init eeprom
    EEPROM.begin(CONFIG_MEMORY_SIZE);

    // run first load
    this->load();

    Logger::info(F("CONF"), F("Successfully initialized"));
}

void ConfigManager::load() {
    unsigned int eeAddress = CONFIG_MEMORY_START;
    uint16_t version;
    uint32_t checksum;

    // read version
    EEPROM.get(eeAddress, version);
    eeAddress += sizeof(version);

    // read checksum
    EEPROM.get(eeAddress, checksum);
    eeAddress += sizeof(checksum);

    if(version != 1) {
        Serial.println(F("Unknown config version!"));
        this->reset();
        return;
    }

    // read config
    EEPROM.get(eeAddress, this->config);

    // calculate checksum
    uint32_t const calculatedChecksum = CRC32::calculate((const uint8_t *)&this->config, sizeof(this->config));
    if(checksum != calculatedChecksum) {
        Serial.println(F("Data corrupted, resetting..."));
        this->reset();
    }
}

void ConfigManager::save() {
    unsigned int eeAddress = CONFIG_MEMORY_START;

    // set version 1
    EEPROM.put(eeAddress, (uint16_t) 1);
    eeAddress += sizeof(uint16_t);

    // calculate and set checksum
    uint32_t const checksum = CRC32::calculate((const uint8_t *)&this->config, sizeof(this->config));

    EEPROM.put(eeAddress, checksum);
    eeAddress += sizeof(checksum);

    // put data
    EEPROM.put(eeAddress, this->config);

    // commit to eeprom
    EEPROM.commit();
}

void ConfigManager::reset() {
    this->config = {0};
    const char * defaultName = this->getDefaultName();
    strcpy(this->config.name, defaultName);

    this->save();
}

char * ConfigManager::getDefaultName() {
    String macAddress = WiFi.macAddress();
    macAddress.toLowerCase();

    String id = "iot-"
        + macAddress.substring(9,11)
        + macAddress.substring(12,14)
        + macAddress.substring(15,17);

    int defaultNameLen = id.length() + 1; 
    // Prepare the character array (the buffer) 
    char defaultName[defaultNameLen];
    id.toCharArray(defaultName, defaultNameLen);

    return defaultName;
}