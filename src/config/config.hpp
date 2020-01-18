#ifndef VENTILATION_CONFIG_H
#define VENTILATION_CONFIG_H

#include <FS.h>
#include <SPIFFS.h>

#define FORMAT_SPIFFS_IF_FAILED true

class config {

    public:
        void init();
        char* getProperty(const char* propertyName);
        void setProperty(const char* propertyName, const char* propertyValue);
    private:
        void loadFile();
        const char* configFile = "/etc/config.properties";
        File conf;
};

#endif