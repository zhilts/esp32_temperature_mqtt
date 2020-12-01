//
// Created by Sergey Zhiltsov on 30.11.2020.
//

#include "MQTTClient.h"
#include <WiFi.h>

MQTTClient::MQTTClient() {
    WiFiClient espClient;
    pubSubClient = new PubSubClient((Client &)espClient);
}

void MQTTClient::reconnect(PubSubClient* clientL) {
    PubSubClient client = *clientL;
//    PubSubClient client = *this->pubSubClient;
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("esp/id", clientId.c_str(), true);
            // ... and resubscribe
            client.subscribe("esp/input");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}


