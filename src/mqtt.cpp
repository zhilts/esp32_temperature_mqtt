//
// Created by Sergey Zhiltsov on 29.11.2020.
//

#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "mqtt.h"
#include "Secrets.h"

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
}

MQTTClient::MQTTClient() {
    espClient = new WiFiClient();
    pubSubClient = new PubSubClient((Client &) *espClient);
}

void MQTTClient::init(String uid) {
    id = "esp32-" + uid;
    Serial.println("Constructor: " + this->id);
    pubSubClient->setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
    pubSubClient->setCallback(callback);
}

void MQTTClient::reconnect() {
    while (!pubSubClient->connected()) {
        Serial.print("Attempting MQTT connection...");
        if (pubSubClient->connect((char *)"ESP8266Client-1234")) {
            Serial.println("connected");
            pubSubClient->publish("esp/id", this->id.c_str(), true);
        } else {
            Serial.print("failed, rc=");
            Serial.print(pubSubClient->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTTClient::disconnect() {
    pubSubClient->disconnect();
}

void MQTTClient::loop() {
    pubSubClient->loop();
}

void MQTTClient::publish(const String &key, const String &value) {
    pubSubClient->publish(("esp/" + key).c_str(), value.c_str(), true);
}
