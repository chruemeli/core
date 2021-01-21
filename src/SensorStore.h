#ifndef SensorStore_h
#define SensorStore_h

#include <Arduino.h>

#include "SensorType.h"
#include "LList.h"

#define SENSOR_NAME_MAX_CHARS 24

union SensorValue {
    int i;
    float f;
    bool b;
};

struct Sensor {
    char name[SENSOR_NAME_MAX_CHARS];
    SensorType type;
    SensorValue value;
};

class SensorStore {
    private:
        LList<Sensor> sensors = LList<Sensor>();

    public:
        Sensor find(char * name) {
            for(int i = 0; i < this->sensors.size(); i++) {
                Sensor s = this->sensors.get(i);
                
                // if name is same, return sensor
                if(strcmp(s.name, name) == 0) {
                    return s;
                }
            }

            return {0};
        }

        LList<Sensor> * findAll() {
            return &this->sensors;
        }

        void put(Sensor sensor) {
            for(int i = 0; i < this->sensors.size(); i++) {
                Sensor s = this->sensors.get(i);

                if(strcmp(sensor.name, s.name) == 0) {
                    // set new data
                    s.type = sensor.type;
                    s.value = sensor.value;

                    return;
                }
            }

            // if we not returned, add as new sensor
            sensors.add(sensor);
        }

        void clear() {
            this->sensors.clear();
        }
};

#endif