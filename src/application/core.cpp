#include "core.hpp"
#include "../ota/ota.hpp"
#include "../network/wlan.hpp"
#include "../config/config.hpp"

ota otaManager = ota();
wlan wlanManager = wlan();
config configManager = config();

void core::init() {

    configManager.init();
    wlanManager.init();
    otaManager.init();
}

void core::run() {

    otaManager.run();
}