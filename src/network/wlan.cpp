#include "wlan.hpp"
#include "../config/config.hpp"
#include <WiFi.h>
#include <string.h>

using namespace std;

void wlan::init() {
    
    string mySSID = "MenMs";
    char ssid[mySSID.length()];
    strcpy(ssid, mySSID.c_str());

    string mySecret = "Welkom 1n d1t huis";
    char networkSecret[mySecret.length()];
    strcpy(networkSecret, mySecret.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, networkSecret);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("connected");

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
    
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