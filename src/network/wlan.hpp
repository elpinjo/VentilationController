#ifndef VENTILATION_WLAN_H
#define VENTILATION_WLAN_H

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WiFiServer.h>
#include <Adafruit_BME280.h>
#include "HTTPRequest.hpp"
#include "../config/config.hpp"

class wlan {

    public:
        void init();
        void run();
        std::string getSSID();
        void updateNetwork(std::string SSID, std::string aNetworkSecret);
        void setSensor(Adafruit_BME280 bme) { bmeSensor = bme;};
    private:
        void startPrivateNetwork();
        void joinConfiguredNetwork();
        String readRequest(WiFiClient &client);
        void reconfigure(HTTPRequest aRequest);

        unsigned long currentTime;
        unsigned long previousTime;
        config configuration;
        WiFiServer* server;
        String rawRequest;
        Adafruit_BME280 bmeSensor;
        std::string ssid;
        std::string networkSecret;

        const unsigned long timeoutTime = 2000;
        const std::string SSID_CONFIG_ITEM = "com.esqr.ventilator.ssid";
        const std::string NETWORK_PASS_CONFIG_ITEM = "com.esqr.ventilator.network.password";
        const char* AP_SSID = "VENTILATOR_AP";
        const char* AP_NETWORK_PASS = "ud8ejr9304";
};

#endif