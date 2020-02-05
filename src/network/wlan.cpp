#include "wlan.hpp"
#include <Arduino.h>
#include "HTTPRequest.hpp"

using namespace std;

wlan::wlan() {

    server = new WiFiServer(80);
    
}

void wlan::init() {

    configuration = config();
    configuration.init();

    ssid = configuration.getProperty(SSID_CONFIG_ITEM);
    networkSecret = configuration.getProperty(NETWORK_PASS_CONFIG_ITEM);
    if ( ssid != NULL && networkSecret != NULL) {
        joinConfiguredNetwork();
    } else {
        startPrivateNetwork();
    }
}

void wlan::run() {

    WiFiClient client = server->available();

    if (client) {

        String rawRequest = "";

        if(client.connected()) {

            String currentLine = "";

            while(client.available()) {

                rawRequest = client.readString();
            }

            client.stop();
        }

        if (!rawRequest.equals("")) {
            
            HTTPRequest myRequest = HTTPRequest(rawRequest);

        }
    }
    
}

const char* wlan::getSSID() {

    return ssid;
}

int wlan::updateNetwork(const char* SSID, const char* networkSecret) {

    configuration.setProperty(SSID_CONFIG_ITEM, SSID);
    configuration.setProperty(NETWORK_PASS_CONFIG_ITEM, networkSecret);
    configuration.saveConfig();
    return 0;
}

//=======================================================================
// Private methods
//=======================================================================

void wlan::reconfigure() {

    
}

void wlan::startPrivateNetwork() {

    WiFi.softAP(AP_SSID, AP_NETWORK_PASS);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    server->begin();
}

void wlan::joinConfiguredNetwork() {

    String mySSID = "MenMs";
    char ssid[mySSID.length()];
    strcpy(ssid, mySSID.c_str());

    String mySecret = "Welkom 1n d1t huis";
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
}