#include "WiFiManager.h"

WifiManager::WifiManager(Scheduler *scheduler, ConfigManager *configManager, StatusStore *statusStore) {
    this->configManager = configManager;
    this->statusStore = statusStore;
    
    WifiManager *wm = this;
    this->cbTaskScanNetworks = std::bind(&WifiManager::taskScanNetworks, this);
    this->cbTaskUpdateMDNS = std::bind(&WifiManager::taskUpdateMDNS, this);

    if(strlen(this->configManager->config.wifi.ssid) == 0) {
        this->runCaptivePortal();
    } else {
        this->runWifiConnection(
            this->configManager->config.wifi.ssid, 
            this->configManager->config.wifi.psk
        );
    }

    MDNS.begin(this->configManager->config.name);

    // add tasks to scheduler
    scheduler->add(new ContinousTask(&this->cbTaskUpdateMDNS));
    scheduler->add(new TimedTask(&this->cbTaskScanNetworks));

    Logger::info(F("WIFI"), F("Successfully initialized"));
}

void WifiManager::handleNetworksScan(int networksFound) {
    StaticJsonDocument<WIFI_LIST_BUFFER_SIZE> wifiNetworks;
    JsonArray wifiList = wifiNetworks.to<JsonArray>();

    for (int i = 0; i < networksFound; i++) {
        JsonObject wifiNetwork = wifiList.createNestedObject();

        wifiNetwork["ssid"] = WiFi.SSID(i);
        wifiNetwork["bssid"] = WiFi.BSSIDstr(i);
        wifiNetwork["rssi"] = WiFi.RSSI(i);
        wifiNetwork["channel"] = WiFi.channel(i);
        wifiNetwork["secure"] = WiFi.encryptionType(i);
    }

    serializeJson(wifiNetworks, this->wifiNetworksBuffer);

    this->isScanning = false;
}

void WifiManager::runCaptivePortal() {
    // reset wifi
    WiFi.persistent(false);
    WiFi.disconnect();
    delay(100);

    // start access point mode
    WiFi.mode(WIFI_AP);
    WiFi.persistent(true);
    WiFi.softAP(this->configManager->config.name);

    Serial.println(F("Opened a captive portal"));
    Serial.println(F("192.168.4.1"));
    
    this->statusStore->isCaptivePortalEnabled = true;
}

void WifiManager::runWifiConnection(const char * ssid, const char * psk) {
    // reset wifi
    this->statusStore->isCaptivePortalEnabled = false;

    WiFi.persistent(false);
    WiFi.disconnect();
    #ifdef ESP8266
    yield();
    #else
    delay(100);
    #endif

    // start access point mode
    WiFi.mode(WIFI_STA);
    WiFi.persistent(true);
    WiFi.begin(ssid, psk, 0, NULL, true);
    #ifdef ESP8266
    yield();
    #else
    delay(2000);
    #endif

    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
        this->runCaptivePortal();
        return;
    }

    strcpy(this->configManager->config.wifi.ssid, ssid);
    strcpy(this->configManager->config.wifi.psk, psk);
    this->configManager->save();

    Serial.println(F("Using local ip:"));
    Serial.println(WiFi.localIP());
}

void WifiManager::setScanningEnabled(bool enabled) {
    // @ TODO on enable set start time and stop after 5 min
    this->isScanningEnabled = enabled;
}

char * WifiManager::getWifiNetworksBuffer() {
    return this->wifiNetworksBuffer;
}

/***** TASKS *****/

void WifiManager::taskUpdateMDNS() {
    MDNS.update();
}

int WifiManager::taskScanNetworks() {
    // scan networks
    if (
        this->isScanningEnabled && !this->isScanning
    ) {
        using namespace std::placeholders;

        WiFi.scanNetworksAsync(std::bind(&WifiManager::handleNetworksScan, this, _1), false);
        this->isScanning = true;
    }

    return WIFI_SCAN_INTERVAL;
}