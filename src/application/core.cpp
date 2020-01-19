#include "core.hpp"
#include "../ota/ota.hpp"
#include "../network/wlan.hpp"

ota otaManager = ota();
wlan wlanManager = wlan();

void core::init() {

    wlanManager.init();
    otaManager.init();
}

void core::run() {

    otaManager.run();
}