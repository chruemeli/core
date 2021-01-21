#ifndef Chruemeli_h
#define Chruemeli_h

#include <LittleFS.h>

#include "Scheduler.h"
#include "Logger.h"
#include "SensorType.h"
#include "SensorStore.h"
#include "ArduinoJson.h"
#include "StatusStore.h"
#include "WifiManager.h"
#include "WebServer.h"
#include "ConfigManager.h"
#include "HTTPRequest.h"

class Chruemeli
{
    private:
        Scheduler *scheduler;
        SensorStore *sensorStore;
        HTTPRequest *httpRequest;
        WifiManager *wifiManager;
        ConfigManager *configManager;
        WebServer *webServer;
        FS *fileSystem;

        StatusStore statusStore;
    public:
        Chruemeli();
        ~Chruemeli();

        void loop();

        void presentData(String sensorName, SensorType type, float value);
        void presentData(String sensorName, SensorType type, int value);
        HTTPRequest * request();

        Scheduler * getScheduler();
};

#endif