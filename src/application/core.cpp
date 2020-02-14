#include "core.hpp"
#include "../network/wlan.hpp"
#include "../ota/ota.hpp"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#ifdef ESP32

#define RELAY0_PIN GPIO_NUM_26

#define I2C_SDA 33
#define I2C_SCL 32

TwoWire I2CBME = TwoWire(0);

#elif defined(ESP8266)
#define RELAY0_PIN D5
#endif

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

// #define SEALEVELPRESSURE_HPA (1013.25)

ota otaManager = ota();
wlan wlanManager = wlan();

bool relayState = HIGH;

void core::init() {

    wlanManager.init();
    otaManager.init();
    pinMode(RELAY0_PIN, OUTPUT);
    digitalWrite(RELAY0_PIN, relayState);

    // default settings
    // (you can also pass in a Wire library object like &Wire2)
#ifdef ESP32
     I2CBME.begin(I2C_SDA, I2C_SCL, 100000);
     bool status = bme.begin(0x76, &I2CBME);
#elif defined(ESP8266)
     bool status = bme.begin(0x76);
#endif
   if (!status) {
       Serial.println("Could not find a valid BME280 sensor, check wiring!");
   }

   wlanManager.setSensor(bme);
}

void core::run() {

    wlanManager.run();
    otaManager.run();
}