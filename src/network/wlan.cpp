#include "wlan.hpp"
#include "../config/config.hpp"

void wlan::init() {
    
    char* content;

    Serial.println(content);
}

char* wlan::getSSID() {

    return "VENTI";
}

void wlan::updateNetwork(char* SSID, char* networkSecret) {

}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
}

void writeFile(fs::FS &fs, const char* path,  const char* content){

    File file = fs.open(path, FILE_WRITE);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for writing");
        return;
    }

    Serial.println("- writing to from file:");
    if(file.print(content)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
}