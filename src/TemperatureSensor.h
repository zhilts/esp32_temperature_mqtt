//
// Created by Sergey Zhiltsov on 29.11.2020.
//

#ifndef TEMPERATURE_TO_MQTT_TEMPERATURESENSOR_H
#define TEMPERATURE_TO_MQTT_TEMPERATURESENSOR_H


class TemperatureSensor {
public:
    TemperatureSensor();
    void init(int pinPlus, int pinMinus);
    float getValue();

private:
    int pinPlus;
    int pinMinus;
    float voltK;
};


#endif //TEMPERATURE_TO_MQTT_TEMPERATURESENSOR_H
