//
// Created by Sergey Zhiltsov on 29.11.2020.
//
#include <Arduino.h>
#include <PubSubClient.h>

#ifndef TEMPERATURE_TO_MQTT_MQTT_H
#define TEMPERATURE_TO_MQTT_MQTT_H

#endif //TEMPERATURE_TO_MQTT_MQTT_H


class MQTTClient {
public:
    MQTTClient();

    void init(String uid);

    void reconnect();

    void publish(const String &key, const String &value);

    void loop();

    void disconnect();

private:
    String id;
    PubSubClient pubSubClient;
};