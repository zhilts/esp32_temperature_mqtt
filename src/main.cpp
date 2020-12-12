#include <Arduino.h>
#include "networking.h"
#include "datetime.h"
#include "mqtt.h"
#include "utils.h"
#include "TemperatureSensor.h"

#define TEMP_PIN_PLUS 34
#define TEMP_PIN_MINUS 35

float temperature = 0;
TemperatureSensor tempSensor;
MQTTClient mqttClient;

void setup() {
    Serial.begin(9600);
    setup_wifi();
    String macAddress = getMacAddress();
    Serial.println("MAC: " + macAddress);
    mqttClient.init(macAddress);
    tempSensor.init(TEMP_PIN_PLUS, TEMP_PIN_MINUS);
}

void loop() {
    temperature = tempSensor.getValue();
    String temperatureStr = floatToString(temperature);
    String now = getNowStr();

    mqttClient.reconnect();
    mqttClient.loop();

    mqttClient.publish("temperature", temperatureStr);
    mqttClient.publish("time", now);
    mqttClient.publish("timer", String(millis()));

    Serial.println(now + ": " + temperatureStr);

    mqttClient.disconnect();

    delay(10 * 60 * 1000); // fixme: replace with timer
//    delay(300);
}