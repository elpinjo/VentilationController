#include <Arduino.h>
#include "application/core.hpp"

core coreApp = core();

void setup() {

  Serial.begin(115200);
  coreApp.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  coreApp.run();
}