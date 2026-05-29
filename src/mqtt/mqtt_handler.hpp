#ifndef VENTILATION_MQTT_HANDLER_H
#define VENTILATION_MQTT_HANDLER_H

#include <string>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include "../config/config.hpp"

class mqtt_handler {
    public:
        void init();
        void run();
        void setSensor(Adafruit_BME280* bme) { bmeSensor = bme; };
        void setMQTTConfig(std::string broker, int port, std::string user, std::string pass, std::string clientId);
        void publishSensorData();
        bool isConnected();
        
    private:
        void connect();
        void reconnect();
        void publishTemperature();
        void publishHumidity();
        void publishPressure();
        
        WiFiClient espClient;
        PubSubClient* mqttClient;
        Adafruit_BME280* bmeSensor;
        config configuration;
        
        std::string mqttBroker;
        int mqttPort;
        std::string mqttUser;
        std::string mqttPassword;
        std::string mqttClientId;
        
        unsigned long lastPublish;
        const unsigned long publishInterval = 60000; // Publish every 60 seconds
        
        // Configuration keys
        const std::string MQTT_BROKER_CONFIG = "com.esqr.ventilator.mqtt.broker";
        const std::string MQTT_PORT_CONFIG = "com.esqr.ventilator.mqtt.port";
        const std::string MQTT_USER_CONFIG = "com.esqr.ventilator.mqtt.user";
        const std::string MQTT_PASS_CONFIG = "com.esqr.ventilator.mqtt.password";
        const std::string MQTT_CLIENT_ID_CONFIG = "com.esqr.ventilator.mqtt.clientid";
        
        // MQTT Topics
        const char* TOPIC_TEMPERATURE = "ventilator/sensor/temperature";
        const char* TOPIC_HUMIDITY = "ventilator/sensor/humidity";
        const char* TOPIC_PRESSURE = "ventilator/sensor/pressure";
        const char* TOPIC_STATUS = "ventilator/status";
};

#endif

// Made with Bob
