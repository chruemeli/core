#include <Arduino.h>
#include <DHTesp.h>
#include <Chruemeli.h>

#define MEASURE_INTERVAL 10000;

DHTesp dht;
Chruemeli * chruemeli;

const int DHT_PIN = D0;

// Measure Callback
TimedTask::Callback measureDHT = []() -> int {
  chruemeli->presentData("temperature", SENSOR_TEMP, dht.getTemperature());
  chruemeli->presentData("humidity", SENSOR_HUMI, dht.getHumidity());

  return MEASURE_INTERVAL;
};

void setup() {
  chruemeli = new Chruemeli();
  chruemeli->getScheduler()->add(new TimedTask(&measureDHT));
  
  dht.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  chruemeli->loop();
}