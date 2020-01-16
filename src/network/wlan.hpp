#ifndef VENTILATION_WLAN_H
#define VENTILATION_WLAN_H

class wlan {

    public:
        void init();
        char* getSSID();
        void updateNetwork(char* SSID, char* networkSecret);
    private:
        
};

#endif