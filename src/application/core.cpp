#include "core.hpp"
#include "../network/wlan.hpp"
#include "../ota/ota.hpp"

ota otaManager = ota();
wlan wlanManager = wlan();

void core::init() {

    wlanManager.init();
    otaManager.init();
}

void core::run() {

    wlanManager.run();
    otaManager.run();
}