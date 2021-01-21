#include "Chruemeli.h"

Chruemeli::Chruemeli() {
    Serial.begin(9600);

    this->sensorStore = new SensorStore();
    this->scheduler = new Scheduler();
    
    // Init status store
    this->statusStore = {
        .isCaptivePortalEnabled = false
    };
    
    this->fileSystem = &LittleFS;

    // Load file system
    LittleFSConfig fileSystemConfig = LittleFSConfig();
    fileSystemConfig.setAutoFormat(true);
    this->fileSystem->setConfig(fileSystemConfig);

    if(!this->fileSystem->begin()) {
        Serial.println(F("Couldn't start LittleFS!"));
        ESP.restart();
        return;
    }

    this->httpRequest = new HTTPRequest();
    this->configManager = new ConfigManager();
    this->wifiManager = new WifiManager(this->scheduler, this->configManager, &this->statusStore);
    this->webServer = new WebServer(this->configManager, this->scheduler, &this->statusStore, this->sensorStore, this->wifiManager);
    
    Logger::info(F("IOTF"), F("Successfully initialized"));
}

Chruemeli::~Chruemeli() {
    delete this->sensorStore;
    delete this->fileSystem;
    delete this->httpRequest;
    delete this->wifiManager;
    delete this->configManager;
    delete this->webServer;
}

void Chruemeli::loop() {
    this->scheduler->execute();
}

void Chruemeli::presentData(String sensorName, SensorType type, float value) {
    Sensor s = Sensor();

    sensorName.toCharArray(s.name, SENSOR_NAME_MAX_CHARS);
    s.type = type;
    s.value.f = value;

    this->sensorStore->put(s);
}

void Chruemeli::presentData(String sensorName, SensorType type, int value) {
    Sensor s = Sensor();

    sensorName.toCharArray(s.name, SENSOR_NAME_MAX_CHARS);
    s.type = type;
    s.value.i = value;

    this->sensorStore->put(s);
}

HTTPRequest * Chruemeli::request() {
    return this->httpRequest;
}

Scheduler* Chruemeli::getScheduler() {
    return this->scheduler;
}