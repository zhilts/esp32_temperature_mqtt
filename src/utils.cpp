//
// Created by Sergey Zhiltsov on 29.11.2020.
//

#include "mqtt.h"
#include "datetime.h"
#include "networking.h"
#include <Arduino.h>
#include "utils.h"

#define NUM_READS 10

float analogReadAvg(int pin) {
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

String floatToString(float value) {
    char tempChar[32];
    memset(tempChar, 0x00, sizeof(tempChar));
    snprintf(tempChar, sizeof(tempChar), "%.2f", value);
    return String(tempChar);
}
