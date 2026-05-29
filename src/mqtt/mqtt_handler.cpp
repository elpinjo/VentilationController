#include "mqtt_handler.hpp"
#include <Arduino.h>

using namespace std;

void mqtt_handler::init() {
    
    configuration = config();
    configuration.init();
    
    // Load MQTT configuration from storage
    mqttBroker = configuration.getProperty(MQTT_BROKER_CONFIG);
    std::string portStr = configuration.getProperty(MQTT_PORT_CONFIG);
    mqttPort = portStr.empty() ? 1883 : atoi(portStr.c_str());
    mqttUser = configuration.getProperty(MQTT_USER_CONFIG);
    mqttPassword = configuration.getProperty(MQTT_PASS_CONFIG);
    mqttClientId = configuration.getProperty(MQTT_CLIENT_ID_CONFIG);
    
    // Set default client ID if not configured
    if (mqttClientId.empty()) {
        mqttClientId = "ventilator_" + std::string(WiFi.macAddress().c_str());
    }
    
    mqttClient = new PubSubClient(espClient);
    
    // Configure MQTT client if broker is set
    if (!mqttBroker.empty()) {
        mqttClient->setServer(mqttBroker.c_str(), mqttPort);
        Serial.print("MQTT configured for broker: ");
        Serial.println(mqttBroker.c_str());
    } else {
        Serial.println("MQTT broker not configured. Use setMQTTConfig() to configure.");
    }
    
    lastPublish = 0;
}

void mqtt_handler::run() {
    
    // Only run if MQTT is configured
    if (mqttBroker.empty()) {
        return;
    }
    
    // Maintain connection
    if (!mqttClient->connected()) {
        reconnect();
    }
    
    if (mqttClient->connected()) {
        mqttClient->loop();
        
        // Publish sensor data at regular intervals
        unsigned long currentMillis = millis();
        if (currentMillis - lastPublish >= publishInterval) {
            publishSensorData();
            lastPublish = currentMillis;
        }
    }
}

void mqtt_handler::setMQTTConfig(std::string broker, int port, std::string user, std::string pass, std::string clientId) {
    
    mqttBroker = broker;
    mqttPort = port;
    mqttUser = user;
    mqttPassword = pass;
    mqttClientId = clientId.empty() ? "ventilator_" + std::string(WiFi.macAddress().c_str()) : clientId;
    
    // Save to configuration
    configuration.setProperty(MQTT_BROKER_CONFIG, broker);
    configuration.setProperty(MQTT_PORT_CONFIG, std::to_string(port));
    configuration.setProperty(MQTT_USER_CONFIG, user);
    configuration.setProperty(MQTT_PASS_CONFIG, pass);
    configuration.setProperty(MQTT_CLIENT_ID_CONFIG, mqttClientId);
    configuration.saveConfig();
    
    // Reconfigure client
    mqttClient->setServer(mqttBroker.c_str(), mqttPort);
    
    Serial.println("MQTT configuration updated");
}

void mqtt_handler::publishSensorData() {
    
    if (!mqttClient->connected()) {
        return;
    }
    
    publishTemperature();
    publishHumidity();
    publishPressure();
}

bool mqtt_handler::isConnected() {
    return mqttClient->connected();
}

//=======================================================================
// Private methods
//=======================================================================

void mqtt_handler::connect() {
    
    if (mqttBroker.empty()) {
        return;
    }
    
    Serial.print("Connecting to MQTT broker: ");
    Serial.println(mqttBroker.c_str());
    
    bool connected = false;
    
    if (!mqttUser.empty() && !mqttPassword.empty()) {
        connected = mqttClient->connect(mqttClientId.c_str(), mqttUser.c_str(), mqttPassword.c_str());
    } else {
        connected = mqttClient->connect(mqttClientId.c_str());
    }
    
    if (connected) {
        Serial.println("MQTT connected");
        
        // Publish online status
        mqttClient->publish(TOPIC_STATUS, "online", true);
    } else {
        Serial.print("MQTT connection failed, rc=");
        Serial.println(mqttClient->state());
    }
}

void mqtt_handler::reconnect() {
    
    static unsigned long lastAttempt = 0;
    unsigned long currentMillis = millis();
    
    // Try to reconnect every 5 seconds
    if (currentMillis - lastAttempt >= 5000) {
        lastAttempt = currentMillis;
        connect();
    }
}

void mqtt_handler::publishTemperature() {
    
    float temperature = bmeSensor->readTemperature();
    
    if (!isnan(temperature)) {
        char tempStr[10];
        dtostrf(temperature, 4, 2, tempStr);
        
        if (mqttClient->publish(TOPIC_TEMPERATURE, tempStr)) {
            Serial.print("Published temperature: ");
            Serial.println(tempStr);
        } else {
            Serial.println("Failed to publish temperature");
        }
    }
}

void mqtt_handler::publishHumidity() {
    
    float humidity = bmeSensor->readHumidity();
    
    if (!isnan(humidity)) {
        char humStr[10];
        dtostrf(humidity, 4, 2, humStr);
        
        if (mqttClient->publish(TOPIC_HUMIDITY, humStr)) {
            Serial.print("Published humidity: ");
            Serial.println(humStr);
        } else {
            Serial.println("Failed to publish humidity");
        }
    }
}

void mqtt_handler::publishPressure() {
    
    float pressure = bmeSensor->readPressure() / 100.0F; // Convert to hPa
    
    if (!isnan(pressure)) {
        char pressStr[10];
        dtostrf(pressure, 6, 2, pressStr);
        
        if (mqttClient->publish(TOPIC_PRESSURE, pressStr)) {
            Serial.print("Published pressure: ");
            Serial.println(pressStr);
        } else {
            Serial.println("Failed to publish pressure");
        }
    }
}

// Made with Bob
