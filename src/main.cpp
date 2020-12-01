#include <Arduino.h>
#include "Secrets.h"
#include "MQTTClient.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define NUM_READS 10

int sensorPinPlus = 34;
int sensorPinMinus = 35;
float sensorValuePlus = 0;
float sensorValueMinus = 0;
float sensorValue = 0;
float adcV = 3.3;
int adcResolutionMax = 4095;
float voltageStep = 0.01;
const char *mqtt_server = "192.168.1.150";

WiFiClient espClient;

PubSubClient clientV = PubSubClient((Client &) espClient);
PubSubClient *clientL = new PubSubClient((Client &) espClient);
MQTTClient *mqttClient;
float voltage = 0;
float temperature = 0;

float analogReadAvg(int pin) {
    // read multiple values and sort them to take the mode
    int sortedValues[NUM_READS];
    for (int i = 0; i < NUM_READS; i++) {
        delay(25);
        int value = analogRead(pin);
        int j;
        if (value < sortedValues[0] || i == 0) {
            j = 0; //insert at first position
        } else {
            for (j = 1; j < i; j++) {
                if (sortedValues[j - 1] <= value && sortedValues[j] >= value) {
                    // j is insert position
                    break;
                }
            }
        }
        for (int k = i; k > j; k--) {
            // move all values higher than current reading up one position
            sortedValues[k] = sortedValues[k - 1];
        }
        sortedValues[j] = value; //insert current reading
    }
    //return scaled mode of 10 values
    float returnval = 0;
    for (int i = NUM_READS / 2 - 5; i < (NUM_READS / 2 + 5); i++) {
        returnval += sortedValues[i];
    }
    return returnval / 10;
}

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
//    PubSubClient client = *clientL;
    PubSubClient client = clientV;
    // Loop until we're reconnected
    Serial.print("Is connected: ");
    Serial.println(client.connected());
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        Serial.println(clientId);
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

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
}

void setup() {
//    PubSubClient client = *clientL;
    PubSubClient client = clientV;
    pinMode(sensorPinPlus, INPUT);
    pinMode(sensorPinMinus, INPUT);
    Serial.begin(9600);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
//    mqttClient = new MQTTClient();
}

String getDateTime() {
    String result = "";
    HTTPClient http;
    http.begin("http://worldclockapi.com/api/json/utc/now");
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        String payload = http.getString();
        StaticJsonDocument<400> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            result = String("deserializeJson() failed: ") + String(error.f_str());
        } else {
            const char *currentDateTime = doc["currentDateTime"];
            result = String(currentDateTime);
        }
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        result = "http error: " + String(httpResponseCode);
    }
    // Free resources
    http.end();
    return result;
}

void loop() {
    PubSubClient client = *clientL;
    sensorValuePlus = analogReadAvg(sensorPinPlus);
    sensorValueMinus = analogReadAvg(sensorPinMinus);
    sensorValue = sensorValuePlus - sensorValueMinus;
    voltage = sensorValue * adcV / (float) adcResolutionMax;
    temperature = voltage / voltageStep;

    char tempChar[32];
    memset(tempChar, 0x00, sizeof(tempChar));
    snprintf(tempChar, sizeof(tempChar), "%.2f", temperature);
    String now = getDateTime();

//    mqttClient->reconnect(clientL);
    Serial.println("before reconnect");
    reconnect();
    Serial.println("after reconnect");
    client.loop();
    client.publish("esp/temperature", tempChar, true);
    client.publish("esp/time", now.c_str(), true);
    client.publish("esp/timer", String(millis()).c_str(), true);

    Serial.println(now + ": " + String(tempChar));

    client.disconnect();

//    delay(10 * 60 * 1000); // fixme: replace with timer
    delay(300);
}