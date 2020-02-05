#include "wlan.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>

using namespace std;

wlan::wlan() {

    server = new WiFiServer(80);
    
}

void wlan::init() {

    configuration = config();
    configuration.init();

    ssid = configuration.getProperty(SSID_CONFIG_ITEM);
    networkSecret = configuration.getProperty(NETWORK_PASS_CONFIG_ITEM);
    Serial.println("ssid found: ");
    Serial.println(ssid);
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

            if (myRequest.getResourcePath().equals("/configNetwork")) {
                reconfigure(myRequest);
            }
        }
    }
    
}

const char* wlan::getSSID() {

    return ssid;
}

void wlan::updateNetwork(const char* SSID, const char* aNetworkSecret) {

    configuration.setProperty(SSID_CONFIG_ITEM, SSID);
    configuration.setProperty(NETWORK_PASS_CONFIG_ITEM, aNetworkSecret);
    configuration.saveConfig();
    ssid = SSID;
    networkSecret = aNetworkSecret;

    joinConfiguredNetwork();
}

//=======================================================================
// Private methods
//=======================================================================

void wlan::reconfigure(HTTPRequest aRequest) {

    if (aRequest.getMethod().equalsIgnoreCase("POST")) {
        StaticJsonDocument<256> jsonDoc;

        DeserializationError error = deserializeJson(jsonDoc, aRequest.getBody());

        // Test if parsing succeeds.
        if (error) {

            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }

        const char* mySSID = jsonDoc["ssid"];
        const char* myNetworkSecret = jsonDoc["networkSecret"];

        Serial.print("Updating network to: ");
        Serial.println(mySSID);
        updateNetwork(mySSID, myNetworkSecret);
    } else if (aRequest.getMethod().equalsIgnoreCase("GET")){

        const char* mySSID = aRequest.getParameterValue("ssid").c_str();
        const char* myNetworkSecret = aRequest.getParameterValue("networkSecret").c_str();

        updateNetwork(mySSID, myNetworkSecret);
    }
}

void wlan::startPrivateNetwork() {

    WiFi.softAP(AP_SSID, AP_NETWORK_PASS);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    server->begin();
}

void wlan::joinConfiguredNetwork() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, networkSecret);

    int retries = 0;
    
    while (WiFi.status() != WL_CONNECTED && retries < 30) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
        retries++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        startPrivateNetwork();
    } else {

        Serial.println("connected");

        // Print ESP32 Local IP Address
        Serial.println(WiFi.localIP());
    }
}