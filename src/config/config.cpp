#include "config.hpp"

void config::init() {

    if (!this->conf) {
        if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){

            Serial.println("SPIFFS Mount Failed");
            ESP.restart();
        }

        this->conf = SPIFFS.open(this->configFile);
        if(!this->conf || this->conf.isDirectory()){
            Serial.println("- failed to open the config file");
            return;
        }
    }
}