#include "config.hpp"
#include <string.h>

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

char* config::getProperty(const char* propertyName) {
    
    if (configProperties == NULL) {
        return NULL;
    } else {
        for (int i=0; i < sizeof(configProperties); i++) {
            if (configProperties[i].name == propertyName) {
                return configProperties[i].value;
            }
        }
        return NULL;
    }
}

void config::setProperty(const char* propertyName, const char* propertyValue) {

}

void config::resetConfig() {


}

void config::loadConfigFile() {

    char content[256];

    if (!conf) {
        conf = SPIFFS.open(configFile);

        if (!conf) {    
            Serial.println("Opening config file failed");
            return;
        }
    }

    while (conf.available()) {
    
        int length = conf.readBytesUntil('\n', content, 256);

        content[length] = 0;
        std::string line = std::string(content);

        int position = line.find('=');

        std::string name = line.substr(0,position);
        std::string value = line.substr(position+1);
    }

    conf.close();
}