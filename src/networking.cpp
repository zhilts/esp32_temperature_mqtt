//
// Created by Sergey Zhiltsov on 29.11.2020.
//

#include <WiFi.h>
#include "Secrets.h"
#include "networking.h"


void setup_wifi() {
    delay(10);
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
}

String getMacAddress() {
    return WiFi.macAddress();
}