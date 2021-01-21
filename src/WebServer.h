#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ESP8266mDNS.h>

#include "Logger.h"
#include "ConfigManager.h"
#include "WifiManager.h"
#include "StatusStore.h"
#include "SensorStore.h"
#include "Scheduler.h"
#include "OneShotTask.h"

#include "generated/data.h"

class WebServer {
    public:
        SensorStore *sensorStore;
        StatusStore *statusStore;
        WifiManager *wifiManager;
        AsyncWebServer *server;
        ConfigManager *configManager;
        Scheduler * scheduler;

        WebServer(ConfigManager * configManager, Scheduler * scheduler, StatusStore * statusStore, SensorStore * sensorStore, WifiManager * wifiManager);
        void loop();
    private:
        void handleRedirect(AsyncWebServerRequest *request, const String & url);
        void handleStatic(AsyncWebServerRequest *request, const String & contentType, const uint8_t * content, size_t contentLength, bool gzipHeader);
        
        void handleStatus(AsyncWebServerRequest *request);

        void handleConfig(AsyncWebServerRequest *request);
        void handleSaveConfig(AsyncWebServerRequest *request, JsonVariant &json);

        void handleWifiList(AsyncWebServerRequest *request);
        void handleSetWifi(AsyncWebServerRequest * request, JsonVariant &json);

        void handleLogs(AsyncWebServerRequest *request);
        
        
        void handleSensorAPI(AsyncWebServerRequest *request);
        void handleSensor(char * name, AsyncWebServerRequest *request);
        void handleSensorList(AsyncWebServerRequest *request);
        void handleDeleteSensorList(AsyncWebServerRequest * request);

        void handleReset(AsyncWebServerRequest *request);
        
        void handlePrometheusApi(AsyncWebServerRequest * request);

        void notFound(AsyncWebServerRequest *request);
};
#endif