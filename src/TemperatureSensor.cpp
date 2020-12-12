//
// Created by Sergey Zhiltsov on 29.11.2020.
//

#include <Arduino.h>
#include "TemperatureSensor.h"
#include "utils.h"

TemperatureSensor::TemperatureSensor() {
    this->voltK = 3.3 / 4095;
}

void TemperatureSensor::init(int _pinPlus, int _pinMinus) {
    this->pinPlus = _pinPlus;
    this->pinMinus = _pinMinus;
    pinMode(this->pinPlus, INPUT);
    pinMode(this->pinMinus, INPUT);
}

float TemperatureSensor::getValue() {
    float sensorValuePlus = analogReadAvg(this->pinPlus);
    float sensorValueMinus = analogReadAvg(this->pinMinus);
    float sensorValue = sensorValuePlus - sensorValueMinus;
    float voltage = sensorValue * this->voltK;
    return voltage / 0.01;
}