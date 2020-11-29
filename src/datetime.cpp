//
// Created by Sergey Zhiltsov on 29.11.2020.
//

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "datetime.h"

String getNowStr() {
    String result = "";
    HTTPClient http;
    http.begin("http://worldclockapi.com/api/json/utc/now");
    http.begin("http://worldtimeapi.org/api/ip");
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        String payload = http.getString();
        StaticJsonDocument<800> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            result = String("deserializeJson() failed: ") + String(error.f_str());
        } else {
            const char *currentDateTime = doc["datetime"];
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