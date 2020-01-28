#ifndef VENTILATION_CONFIG_H
#define VENTILATION_CONFIG_H

#include <FS.h>
#include <SPIFFS.h>
#include <map>

#define FORMAT_SPIFFS_IF_FAILED true

class config {

    public:
        void init();
        const char* getProperty(const char* propertyName);
        void setProperty(const char* propertyName, const char* propertyValue);
        void resetConfig();
        void saveConfig();
    private:
        void openFile(const std::string mode);
        void loadFile();
        void updateFile();
        void closeFile();
        void deleteFile();
        const char* configFile = "/etc/config.properties";
        File conf;
        std::map<std::string, std::string> configProperties;
};

#endif