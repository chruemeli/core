#include <Arduino.h>
#include <Chruemeli.h>

Chruemeli * chruemeli;

void setup() {
  chruemeli = new Chruemeli();
}

void loop() {
  chruemeli->loop();
}