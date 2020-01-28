#include "config.hpp"
#include <iterator>

//==========================================================================================
// public methods
//==========================================================================================

void config::init() {

    openFile(FILE_READ);
    loadFile();
    closeFile();
}

const char* config::getProperty(const char* propertyName) {

    std::map<std::string, std::string>::iterator iter = configProperties.find(propertyName);
    
    if (iter == configProperties.end()) {
        return NULL;
    } else {
        return iter->second.c_str();
    }
}

void config::setProperty(const char* propertyName, const char* propertyValue) {

    configProperties[propertyName] = propertyValue;
}

void config::resetConfig() {

    std::map<std::string, std::string>::iterator iter = configProperties.begin();
    while(iter != configProperties.end())
    {
        configProperties.erase(iter);
        iter++;
    }

    deleteFile();
}

void config::saveConfig() {
    
    updateFile();
}

//==========================================================================================
// private methods
//==========================================================================================

void config::loadFile() {

    if (!conf) {
        openFile(FILE_READ);
    }

    char buffer[1024];
    while (conf.available()) {
        
        int l = conf.readBytesUntil('\n', buffer, sizeof(buffer));
        buffer[l] = 0;
        Serial.println(buffer);
    }
}

void config::openFile(const std::string mode) {

    if (!conf) {
        if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){

            Serial.println("SPIFFS Mount Failed");
            ESP.restart();
        }

        conf = SPIFFS.open(configFile, mode.c_str());
        
        if(!conf || conf.isDirectory()){
            Serial.println("- failed to open the config file");
        }
    }
}

void config::updateFile(){

    deleteFile();

    if (!conf) {
        openFile(FILE_WRITE);
    }

    std::map<std::string, std::string>::iterator iter = configProperties.begin();
    while(iter != configProperties.end())
    {
        conf.print(iter->first.c_str());
        conf.print('=');
        conf.println(iter->second.c_str());
        iter++;
    }
}

void config::deleteFile() {

    if (conf) {
        closeFile();
    }

    SPIFFS.remove(configFile);
}

void config::closeFile() {
    
    if (conf) {
        conf.close();
    }
}