#include <Arduino.h>


int sensorPinPlus = 13;
int sensorPinMinus = 12;
float sensorValuePlus = 0;
float sensorValueMinus = 0;
float sensorValue = 0;
float adcV = 3.3;
int adcResolutionMax = 4095;
float voltageStep = 0.01;

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

void setup() {
    pinMode(sensorPinPlus, INPUT);
    pinMode(sensorPinMinus, INPUT);
    Serial.begin(9600);
}

void loop() {
    sensorValuePlus = analogReadAvg(sensorPinPlus);
    sensorValueMinus = analogReadAvg(sensorPinMinus);
    sensorValue = sensorValuePlus - sensorValueMinus;
    voltage = sensorValue * adcV / adcResolutionMax;
    temperature = voltage / voltageStep;
    Serial.println(temperature);
    delay(1000);
}