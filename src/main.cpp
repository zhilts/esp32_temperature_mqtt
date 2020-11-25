#include <Arduino.h>
#include <Secrets.h>
#include <WiFi.h>
#include <PubSubClient.h>

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
PubSubClient client(espClient);

float voltage = 0;
float temperature = 0;

int filterLen = 1000;

float analogReadAvg(int pin) {
    float sum = 0;
    for (int i = 0; i < filterLen; i++) {
        sum += analogRead(pin);
    }
    return sum / filterLen;
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
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
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

    // Switch on the LED if an 1 was received as first character
    if ((char) payload[0] == '1') {
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is active low on the ESP-01)
        Serial.println("Set low");
    } else {
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
        Serial.println("Set hight");
    }

}

void setup() {
    pinMode(sensorPinPlus, INPUT);
    pinMode(sensorPinMinus, INPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    Serial.begin(9600);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    sensorValuePlus = analogReadAvg(sensorPinPlus);
    sensorValueMinus = analogReadAvg(sensorPinMinus);
    sensorValue = sensorValuePlus - sensorValueMinus;
    voltage = sensorValue * adcV / adcResolutionMax;
    temperature = voltage / voltageStep;
    Serial.println(temperature);

    char buffer[32];
    memset(buffer, 0x00, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%.4f", temperature);
    client.publish("temp", buffer);

    delay(3000);
}