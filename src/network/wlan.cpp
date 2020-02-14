#include "wlan.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP32
#define RELAY_PIN GPIO_NUM_26
#elif defined(ESP8266)
#define RELAY_PIN D5
#endif

using namespace std;

void wlan::init() {

    server = new WiFiServer(80);
    currentTime = millis();
    previousTime = 0;
    configuration = config();
    configuration.init();

    ssid = configuration.getProperty(SSID_CONFIG_ITEM);
    networkSecret = configuration.getProperty(NETWORK_PASS_CONFIG_ITEM);

    if ( !ssid.empty() && !networkSecret.empty()) {
        joinConfiguredNetwork();
    } else {
        startPrivateNetwork();
    }
}

void wlan::run() {

    WiFiClient client = server->available();

    if (client) {

        Serial.println("client connected.");
        
        rawRequest = readRequest(client);

        //Serial.println("Received request");
        //Serial.println(rawRequest);

        if (!rawRequest.equals("")) {
            
            HTTPRequest myRequest = HTTPRequest(rawRequest);

            if (myRequest.getResourcePath().equals("/configNetwork")) {
                reconfigure(myRequest);

                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                client.stop();
            } else if (myRequest.getResourcePath().equals("/off")) {
                digitalWrite(RELAY_PIN, LOW);

                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                client.println("<html><head><title>Environment</title></head><body><center><H1>The ventilator has been stopped</h1></center></body>");
                client.println();
                client.stop();
                Serial.println("close client connection");
            }  else if (myRequest.getResourcePath().equals("/on")) {
                digitalWrite(RELAY_PIN, HIGH);
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                client.println("<html><head><title>Environment</title></head><body><center><H1>The ventilator has been turned on</h1></center></body>");
                client.println();
                client.stop();

                Serial.println("close client connection");
            } else if (myRequest.getResourcePath().equals("/temperature")) {
                float temperature = bmeSensor.readTemperature();
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                client.print("<html><head><title>Environment</title></head><body><center><H1>The temperature is:</h1><br><H1>");
                client.print(temperature);
                client.println("</h1></center></body>");
                client.println();

                client.stop();
            } else if (myRequest.getResourcePath().equals("/reset")) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                ESP.restart();
            } else {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                client.print("<html><head><title>Environment</title></head><body><center><H1>Unknown command: ");
                client.print(myRequest.getResourcePath());
                client.println("</h1</center></body>");
                client.println();
                client.stop();
            }

        }
        client.flush();
        client.stop();
    }
    
}

std::string wlan::getSSID() {

    return ssid;
}

void wlan::updateNetwork(std::string SSID, std::string aNetworkSecret) {

    configuration.setProperty(SSID_CONFIG_ITEM, SSID);
    configuration.setProperty(NETWORK_PASS_CONFIG_ITEM, aNetworkSecret);
    configuration.saveConfig();
    ssid = SSID;
    networkSecret = aNetworkSecret;

    joinConfiguredNetwork();
}

//=======================================================================
// Private methods
//=======================================================================

void wlan::reconfigure(HTTPRequest aRequest) {

    if (aRequest.getMethod().equalsIgnoreCase("POST")) {
        StaticJsonDocument<256> jsonDoc;

        DeserializationError error = deserializeJson(jsonDoc, aRequest.getBody());

        // Test if parsing succeeds.
        if (error) {

            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }

        String mySSID = jsonDoc["ssid"];

        String myNetworkSecret = jsonDoc["networkSecret"];

        updateNetwork(std::string(mySSID.c_str()), std::string(myNetworkSecret.c_str()));
    } else if (aRequest.getMethod().equalsIgnoreCase("GET")){

        const char* mySSID = aRequest.getParameterValue("ssid").c_str();
        const char* myNetworkSecret = aRequest.getParameterValue("networkSecret").c_str();

        updateNetwork(std::string(mySSID), std::string(myNetworkSecret));
    }
}

String wlan::readRequest(WiFiClient &client) {

    String rawRequest = "";
    String currentLine = "";
    
    currentTime = millis();
    previousTime = currentTime;
    char c;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // if there's bytes to read from the client,
            c = client.read();             // read a byte, then
            //Serial.write(c);
            rawRequest += c;
            if (c == '\n') {                    // if the byte is a newline character
                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP Header;
                if (currentLine.length() != 0) {
                    currentLine = "";
                }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
                currentLine += c;      // add it to the end of the currentLine
            }
        }
    }

    return rawRequest;

}

void wlan::startPrivateNetwork() {

    WiFi.softAP(AP_SSID, AP_NETWORK_PASS);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    server->begin();
}

void wlan::joinConfiguredNetwork() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), networkSecret.c_str());

    int retries = 0;
    
    Serial.print("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED && retries < 30) {
        delay(1000);
        Serial.print(".");
        retries++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        startPrivateNetwork();
    } else {

        Serial.println("connected");

        // Print ESP32 Local IP Address
        Serial.println(WiFi.localIP());
    }

    server->begin();
}