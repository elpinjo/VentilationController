#ifndef VENTILATION_WLAN_H
#define VENTILATION_WLAN_H

const char* ssid = "MenMs";
const char* password = "Welkom 1n d1t huis";

class wlan {

    public:
        void init();
        char* getSSID();
        void updateNetwork(char* SSID, char* networkSecret);
    private:
        char* ssid;
        char* networkSecret;
};

#endif