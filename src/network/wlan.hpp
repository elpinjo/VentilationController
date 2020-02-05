#ifndef VENTILATION_WLAN_H
#define VENTILATION_WLAN_H

#include <WiFi.h>
#include <WiFiServer.h>
#include "HTTPRequest.hpp"
#include "../config/config.hpp"

class wlan {

    public:
        wlan();
        void init();
        void run();
        const char* getSSID();
        void updateNetwork(const char* SSID, const char* aNetworkSecret);
    private:
        void startPrivateNetwork();
        void joinConfiguredNetwork();
        void reconfigure(HTTPRequest aRequest);
        config configuration;
        WiFiServer* server;
        const char* ssid;
        const char* networkSecret;
        std::function<void(void)> getFunction;

        const char* SSID_CONFIG_ITEM = "com.esqr.ventilator.ssid";
        const char* NETWORK_PASS_CONFIG_ITEM = "com.esqr.ventilator.network.password";
        const char* AP_SSID = "VENTILATOR_AP";
        const char* AP_NETWORK_PASS = "ud8ejr9304";
};

#endif