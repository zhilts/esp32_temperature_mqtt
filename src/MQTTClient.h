//
// Created by Sergey Zhiltsov on 30.11.2020.
//

#include <PubSubClient.h>

#ifndef TEMPERATURE_TO_MQTT_MQTTCLIENT_H
#define TEMPERATURE_TO_MQTT_MQTTCLIENT_H


class MQTTClient {
public:
    MQTTClient();
    void reconnect();

private:
    PubSubClient * pubSubClient;
};


#endif //TEMPERATURE_TO_MQTT_MQTTCLIENT_H
