#include "config.hpp"
#include <iterator>

//==========================================================================================
// public methods
//==========================================================================================

config::config() {
}

void config::init() {

    Serial.println("Constructor for COnfig");
#ifdef ESP32
        if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
#elif defined(ESP8266)
        if(!SPIFFS.begin()){
            Serial.println("No SPIFFS, begin to format");
            SPIFFS.format();
#endif
            Serial.println("SPIFFS Mount Failed");
            ESP.restart();
        }

    openFile("r");
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
        openFile("r");
    }

    char buffer[1024];
    while (conf.available()) {
        
        int l = conf.readBytesUntil('\n', buffer, sizeof(buffer));
        buffer[l-1] = 0;
        std::string configItem(buffer);

        std::size_t found=configItem.find("=");
        if (found!=std::string::npos) {
            std::string property = configItem.substr(0,found);
            std::string value = configItem.substr(found+1);

            configProperties[property] = value;
        }
    }
}

void config::openFile(const std::string mode) {

    if (!conf) {

        conf = SPIFFS.open(configFile, mode.c_str());
        
        if(!conf || conf.isDirectory()){
            Serial.println("- failed to open the config file");
            #ifdef ESP8266
            Serial.println("No config file, begin to format");
            SPIFFS.format();
            conf = SPIFFS.open(configFile, "w");
            #endif
        }
    }
}

void config::updateFile(){

    deleteFile();

    if (!conf) {
        openFile("w");
    }

    std::map<std::string, std::string>::iterator iter = configProperties.begin();
    while(iter != configProperties.end())
    {
        conf.print(iter->first.c_str());
        conf.print('=');
        conf.println(iter->second.c_str());
        iter++;
    }
    
    closeFile();
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