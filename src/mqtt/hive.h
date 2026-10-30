#pragma once

#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <DHT.h>
#include "config.h"

class HiveLocal
{
public:
    void begin();
    void update();

private:
    const char ssid[sizeof(WIFI_SSID)] = WIFI_SSID;
    const char pass[sizeof(WIFI_PASS)] = WIFI_PASS;

    IPAddress broker{10, 131, 15, 56};
    const int port = 1883;

    WiFiClient wifiClient;
    MqttClient mqttClient{wifiClient};
    DHT dht{DHT_PIN, DHT_TYPE};

    unsigned long previousMillis = 0;

    void connectWiFi();

    void connectMQTT();
};