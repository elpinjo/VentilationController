#include "ota.hpp"
#include <Arduino.h>

#include <ArduinoOTA.h>

void ota::init() {
    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    });

    ArduinoOTA.onEnd([]() {
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    });

    ArduinoOTA.onError([](ota_error_t error) {
    });

    ArduinoOTA.begin();
}

void ota::run() {
    ArduinoOTA.handle();
}