#ifndef VENTILATION_CONFIG_H
#define VENTILATION_CONFIG_H

#include <FS.h>
#include <SPIFFS.h>

#define FORMAT_SPIFFS_IF_FAILED true

struct properties {
    char* name;
    char* value;
};

class config {

    public:
        void init();
        char* getProperty(const char* propertyName);
        void setProperty(const char* propertyName, const char* propertyValue);
        void resetConfig();
    private:
        void loadConfigFile();
        File createConfigFile();
        void deleteConfigFile();
        const char* configFile = "/etc/config.properties";
        File conf;
        properties configProperties[];
};

#endif