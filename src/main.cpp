#include <Arduino.h>

int sensorPin = 34;
int sensorValue = 0;
float vcc = 4.64;
float dividerK = 1.438 / 2.328;
float adcV = vcc * dividerK;
int adcResolutionMax = 4095;
float voltageStep = 0.01 * dividerK; // 10mV / degree - LM35

float voltage = 0;
float temperature = 0;

void setup() {
    Serial.begin(9600);
    pinMode(sensorPin, INPUT);
}

void loop() {
    sensorValue = analogRead(sensorPin);
    Serial.print("Value: ");
    Serial.print(sensorValue);
    Serial.print("; Voltage: ");
    voltage = sensorValue * adcV / adcResolutionMax;
    Serial.print(voltage);
    Serial.print("; Temperature: ");
    temperature = voltage / voltageStep;
    Serial.print(temperature);
    Serial.println("");
    delay(1000);
}