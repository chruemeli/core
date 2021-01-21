#include "WebServer.h"

WebServer::WebServer(ConfigManager * configManager, Scheduler * scheduler, StatusStore * statusStore, SensorStore * sensorStore, WifiManager * wifiManager) {
    this->scheduler = scheduler;
    this->statusStore = statusStore;
    this->sensorStore = sensorStore;
    this->wifiManager = wifiManager;
    this->configManager = configManager;

    this->server = new AsyncWebServer(80);

    {
        using namespace std::placeholders;

        this->server->onNotFound(std::bind(&WebServer::notFound, this, _1));

        // webui redirects
        this->server->on("/", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/index.html")));
        this->server->on("/dashboard", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/index.html")));
        this->server->on("/wifi", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/index.html")));
        this->server->on("/settings", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/index.html")));
        this->server->on("/logs", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/index.html")));
        this->server->on("/about", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/index.html")));

        // serve index.html
        this->server->on("/index.html", HTTP_GET, std::bind(&WebServer::handleStatic, this, _1, F("text/html"), indexHtml, indexHtmlLen, false));

        // redirect main.css and server main.css.gz
        this->server->on("/main.css", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/main.css.gz")));
        this->server->on("/main.css.gz", HTTP_GET, std::bind(&WebServer::handleStatic, this, _1, F("text/css"), mainCssGz, mainCssGzLen, true));

        // redirect main.js and server main.js.gz
        this->server->on("/main.js", HTTP_GET, std::bind(&WebServer::handleRedirect, this, _1, F("/main.js.gz")));
        this->server->on("/main.js.gz", HTTP_GET, std::bind(&WebServer::handleStatic, this, _1, F("text/javascript"), mainJsGz, mainJsGzLen, true));
    
        // Status API
        this->server->on("/api/status", HTTP_GET, std::bind(&WebServer::handleStatus, this, _1));

        // Sensor API
        this->server->on("/api/sensor", HTTP_GET, std::bind(&WebServer::handleSensorAPI, this, _1));
        this->server->on("/api/sensor", HTTP_DELETE, std::bind(&WebServer::handleDeleteSensorList, this, _1));

        // Prometheus API
        this->server->on("/api/prometheus", HTTP_GET, std::bind(&WebServer::handlePrometheusApi, this, _1));

        // Config API
        this->server->on("/api/config", HTTP_GET, std::bind(&WebServer::handleConfig, this, _1));
        this->server->addHandler(
            new AsyncCallbackJsonWebHandler("/api/config", std::bind(&WebServer::handleSaveConfig, this, _1, _2))
        );

        // Wifi API
        this->server->on("/api/wifi", HTTP_GET, std::bind(&WebServer::handleWifiList, this, _1));
        this->server->addHandler(
            new AsyncCallbackJsonWebHandler("/api/wifi", std::bind(&WebServer::handleSetWifi, this, _1, _2))
        );

        // Logs API
        this->server->on("/api/logs", HTTP_GET, std::bind(&WebServer::handleLogs, this, _1));

        // Reset API
        this->server->on("/api/reset", HTTP_POST, std::bind(&WebServer::handleLogs, this, _1));
    }

    this->server->begin();

    // advertise webserver via mdns
    MDNS.addService("http", "tcp", 80);

    Logger::info(F("WEB"), F("Successfully initialized"));
}

void WebServer::loop() {}

/**
 * Redirect handler
 */
void WebServer::handleRedirect(AsyncWebServerRequest *request, const String & url) {
    request->redirect(url);
}

/**
 * Static handler
 */
void WebServer::handleStatic(AsyncWebServerRequest *request, const String & contentType, const uint8_t * content, size_t contentLength, bool gzipHeader) {
    // Dump the byte array in PROGMEM with a 200 HTTP code (OK)
    AsyncWebServerResponse *response = request->beginResponse_P(200, contentType, content, contentLength);

    // send gzip header if necessary
    if(gzipHeader) response->addHeader(F("Content-Encoding"), F("gzip"));
    
    request->send(response);
}

void WebServer::handleStatus(AsyncWebServerRequest *request) {
    AsyncJsonResponse *response = new AsyncJsonResponse();
    
    const JsonObject &root = response->getRoot();
    root["isCaptivePortalEnabled"] = this->statusStore->isCaptivePortalEnabled;

    response->setLength();
    request->send(response);
}

void WebServer::handleConfig(AsyncWebServerRequest *request) {
    AsyncJsonResponse *response = new AsyncJsonResponse();

    const JsonObject &root = response->getRoot();
    root["name"] = this->configManager->config.name;

    response->setLength();
    request->send(response);
}

void WebServer::handleSaveConfig(AsyncWebServerRequest *request, JsonVariant &json) {
    const JsonObject& config = json.as<JsonObject>();
    bool restart = false;

    if (config.containsKey("name")) {
        const char * name = config["name"];
        
        strcpy(this->configManager->config.name, name);
        this->configManager->save();

        Logger::info(F("WEB"), "Renamed device: %s", name);

        restart = true;
    }

    if (config.containsKey("scan-wifi")) {
        bool scanWifi = config["scan-wifi"];
        this->wifiManager->setScanningEnabled(scanWifi);
    }
    
    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonObject &res = response->getRoot();

    res["msg"] = F("Settings saved!");
    res["restart"] = restart;

    response->setLength();
    request->send(response);

    // restart via scheduler task
    if (restart) {
        this->scheduler->add(new OneShotTask(
            []() -> void {
                ESP.restart();
            }
        ));
    }
}

void WebServer::handleWifiList(AsyncWebServerRequest *request) {
    request->send(200, F("application/json"), this->wifiManager->getWifiNetworksBuffer());
}

void WebServer::handleLogs(AsyncWebServerRequest *request) {
    String filePath = Logger::instance().getLogFile();

    if(LittleFS.exists(filePath)) {
        request->send(LittleFS, filePath, F("text/plain"));
    } else {
        request->send(404, F("{\"msg\": \"Log file not found!\"}"), F("application/json"));
    }
}

void WebServer::handleSensorAPI(AsyncWebServerRequest *request) {
    char urlBuf[64];
    request->url().toCharArray(urlBuf, 64);

    if(strcmp(urlBuf, "/api/sensor") == 0) {
        this->handleSensorList(request);
    } else {
        char sensorName[64];
        request->url().substring(12).toCharArray(sensorName, 64);

        this->handleSensor(sensorName, request);
    }
}

void WebServer::handleSensorList(AsyncWebServerRequest *request) {
    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonArray &res = response->getRoot().to<JsonArray>();

    LList<Sensor> * sensors = this->sensorStore->findAll();
    for(int i = 0; i < sensors->size(); i++) {
        Sensor s = sensors->get(i);
        const JsonObject &resObj = res.createNestedObject();

        resObj["name"] = s.name;
        switch(s.type) {
            case SensorType::SENSOR_TEMP:
                resObj["type"] = F("temperature");
                resObj["value"] = s.value.f;
                break;
            case SensorType::SENSOR_HUMI:
                resObj["type"] = F("humidity");
                resObj["value"] = s.value.f;
                break;
            case SensorType::SENSOR_FLOAT:
                resObj["type"] = F("float");
                resObj["value"] = s.value.f;
                break;
            default:
                continue;
        }
    }

    response->setLength();
    request->send(response);
}

void WebServer::handleSensor(char * name, AsyncWebServerRequest * request) {
    Sensor s = this->sensorStore->find(name);
    // if not found
    if (strcmp(s.name, "") == 0) {
        this->notFound(request);
        return;
    }

    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonObject &res = response->getRoot();

    res["name"] = s.name;
    switch(s.type) {
        case SensorType::SENSOR_TEMP:
            res["type"] = F("temperature");
            res["value"] = s.value.f;
            break;
        case SensorType::SENSOR_HUMI:
            res["type"] = F("humidity");
            res["value"] = s.value.f;
            break;
        case SensorType::SENSOR_FLOAT:
            res["type"] = F("float");
            res["value"] = s.value.f;
            break;
        case SensorType::SENSOR_INTEGER:
            res["type"] = F("integer");
            res["value"] = s.value.i;
            break;
    }
    
    response->setLength();
    request->send(response);
}

void WebServer::handleDeleteSensorList(AsyncWebServerRequest * request) {
    char urlBuf[64];
    request->url().toCharArray(urlBuf, 64);

    // if we are not direct adressing, show 404;
    if(strcmp(urlBuf, "/api/sensor") != 0) {
        this->notFound(request);
        return;
    }

    this->sensorStore->clear();

    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonObject &res = response->getRoot();
     
    res["msg"] = F("Sensor data deleted!");

    response->setLength();
    request->send(response);
}

void WebServer::handleSetWifi(AsyncWebServerRequest * request, JsonVariant &json) {
    const JsonObject& wifiConfig = json.as<JsonObject>();
    const char * ssid = wifiConfig["ssid"];
    const char * psk = wifiConfig["psk"];
    
    if(ssid != NULL && psk != NULL) {
        WifiManager *wm = this->wifiManager;

        // update wifi via scheduler, that we can end the request
        this->scheduler->add(new OneShotTask(
            [ssid, psk, wm]() -> void {
                wm->runWifiConnection(ssid, psk);
            }
        ));

        Logger::info(F("WEB"), "Configured wifi: %s", ssid);
    }

    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonObject &res = response->getRoot();

    res["msg"] = F("Wifi settings saved!");

    response->setLength();
    request->send(response);
}

void WebServer::handlePrometheusApi(AsyncWebServerRequest * request) {
    AsyncResponseStream *response = request->beginResponseStream("text/plain");

    LList<Sensor> * sensors = this->sensorStore->findAll();
    for(int i = 0; i < sensors->size(); i++) {
        Sensor s = sensors->get(i);

        switch(s.type) {
            case SensorType::SENSOR_TEMP:
                response->printf("%s{type=\"%s\",device_name=\"%s\"} %4.2f\n", s.name, F("temperature"), this->configManager->config.name, s.value.f);
                break;
            case SensorType::SENSOR_HUMI:
                response->printf("%s{type=\"%s\",device_name=\"%s\"} %4.2f\n", s.name, F("humidity"), this->configManager->config.name, s.value.f);
                break;
            case SensorType::SENSOR_FLOAT:
                response->printf("%s{type=\"%s\",device_name=\"%s\"} %f\n", s.name, F("float"), this->configManager->config.name, s.value.f);
                break;
            case SensorType::SENSOR_INTEGER:
                response->printf("%s{type=\"%s\",device_name=\"%s\"} %d\n", s.name, F("integer"), this->configManager->config.name, s.value.i);
                break;
        }
    }

    request->send(response);
}

void WebServer::handleReset(AsyncWebServerRequest *request) {
    char urlBuf[64];
    request->url().toCharArray(urlBuf, 64);

    // if we are not direct adressing, show 404;
    if(strcmp(urlBuf, "/api/reset") != 0) {
        this->notFound(request);
        return;
    }

    this->configManager->reset();
    
    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonObject &res = response->getRoot();

    res["msg"] = "Reset done!";

    response->setLength();
    request->send(response);

    this->scheduler->add(new OneShotTask(
        []() -> void {
            ESP.restart();
        }
    ));
}

void WebServer::notFound(AsyncWebServerRequest *request) {
    AsyncJsonResponse *response = new AsyncJsonResponse();
    const JsonObject &res = response->getRoot();

    res["msg"] = "Not found!";

    response->setCode(404);
    response->setLength();
    request->send(response);
}