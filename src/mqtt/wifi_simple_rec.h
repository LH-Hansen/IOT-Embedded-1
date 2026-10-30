#pragma once

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "config.h"

class WifiSimpleRec
{
public:
    void begin()
    {
        Serial.begin(9600);
        while (!Serial)
            ;

        // Connect to WiFi
        Serial.print("Connecting to WiFi");

        while (WiFi.begin(ssid, pass) != WL_CONNECTED)
        {
            Serial.print(".");
            delay(3000);
        }

        Serial.println("\nWiFi connected!");

        // MQTT authentication
        mqttClient.setId("mkr-receiver-12345");
        mqttClient.setUsernamePassword("public", "public");

        // Connect to broker
        Serial.print("Connecting to MQTT broker...");

        if (!mqttClient.connect(broker, port))
        {
            Serial.print("Failed! Error: ");
            Serial.println(mqttClient.connectError());
            while (true)
                ;
        }

        Serial.println("Connected!");

        // Subscribe
        mqttClient.subscribe(topic);

        Serial.print("Subscribed to: ");
        Serial.println(topic);
    }
    void update()
    {
        int messageSize = mqttClient.parseMessage();

        if (messageSize)
        {
            Serial.print("Received message: ");

            while (mqttClient.available())
            {
                Serial.print((char)mqttClient.read());
            }

            Serial.println();
        }
    }

private:
    const char ssid[sizeof(WIFI_SSID)] = WIFI_SSID;
    const char pass[sizeof(WIFI_PASS)] = WIFI_PASS;

    const char broker[sizeof("public.cloud.shiftr.io")] = "public.cloud.shiftr.io";
    const int port = 1883;
    const char topic[sizeof("arduino/simple")] = "arduino/simple";

    WiFiClient wifiClient;
    MqttClient mqttClient{wifiClient};
};