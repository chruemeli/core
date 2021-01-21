#ifndef WifiManager_h
#define WifiManager_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "Scheduler.h"
#include "ContinousTask.h"
#include "TimedTask.h"
#include "LList.h"
#include "Logger.h"
#include "StatusStore.h"
#include "ConfigManager.h"

#define WIFI_LIST_FILE "/sensors/wifi"
#define WIFI_SCAN_INTERVAL 20000
#define WIFI_LIST_BUFFER_SIZE 2048

class WifiManager
{
    private:
        ConfigManager *configManager;
        StatusStore *statusStore;

        std::function<int()> cbTaskScanNetworks;
        std::function<void()> cbTaskUpdateMDNS;

        char wifiNetworksBuffer[WIFI_LIST_BUFFER_SIZE];

        bool isScanningEnabled = false;
        bool isScanning = false;

        void runCaptivePortal();
        void handleNetworksScan(int networksFound);

        int taskScanNetworks();
        void taskUpdateMDNS();

    public:
        WifiManager(Scheduler *scheduler, ConfigManager *configManager, StatusStore *statusStore);
        
        void runWifiConnection(const char * ssid, const char * psk);
        void setScanningEnabled(bool enabled);

        char * getWifiNetworksBuffer();
};

#endif