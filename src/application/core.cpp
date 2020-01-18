#include "core.hpp"
#include "../ota/ota.hpp"

ota otaManager = ota();

void core::init() {
    
    otaManager.init();
}

void core::run() {

    otaManager.run();
}