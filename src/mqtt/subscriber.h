
#pragma once

#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Servo.h>
#include "config.h"

class Subscriber
{
public:
    void begin()
    {
        Serial.begin(9600);
        while (!Serial && millis() < 3000);

        // Initialize LED and servo
        pinMode(LED_PIN, OUTPUT);
        digitalWrite(LED_PIN, LOW);

        servo.attach(6);
        servo.write(0);

        // Connect to WiFi
        Serial.print("Connecting to WiFi");

        while (WiFi.begin(ssid, pass) != WL_CONNECTED)
        {
            Serial.print(".");
            delay(3000);
        }

        Serial.println("\nWiFi connected!");

        // MQTT authentication
        mqttClient.setId("mkr-led-servo-12345");
        mqttClient.setUsernamePassword("public", "public");

        Serial.print("Connecting to MQTT broker...");

        if (!mqttClient.connect(broker, port))
        {
            Serial.print("Failed! Error: ");
            Serial.println(mqttClient.connectError());
            while (true);
        }

        Serial.println("Connected!");

        // Register callback
        activeInstance() = this;
        mqttClient.onMessage(onMqttMessage);

        // Subscribe to topic
        mqttClient.subscribe(topic);

        Serial.print("Subscribed to: ");
        Serial.println(topic);
    }

    void update()
    {
        mqttClient.poll();
    }

private:
    const char ssid[sizeof(WIFI_SSID)] = WIFI_SSID;
    const char pass[sizeof(WIFI_PASS)] = WIFI_PASS;

    const char broker[sizeof("public.cloud.shiftr.io")]
        = "public.cloud.shiftr.io";

    const int port = 1883;
    const char topic[sizeof("arduino/simple")]
        = "arduino/simple";

    WiFiClient wifiClient;
    MqttClient mqttClient{wifiClient};
    Servo servo;

    // Route static callback to this class instance
    static Subscriber*& activeInstance()
    {
        static Subscriber* instance = nullptr;
        return instance;
    }

    static void onMqttMessage(int messageSize)
    {
        Subscriber* instance = activeInstance();

        if (instance != nullptr)
        {
            instance->handleMessage(messageSize);
        }
    }

    void handleMessage(int messageSize)
    {
        // Read MQTT payload
        String message = mqttClient.readString();
        message.trim();

        Serial.print("Received: ");
        Serial.println(message);

        if (message == "on")
        {
            digitalWrite(LED_PIN, HIGH);
            servo.write(180);

            Serial.println("LED ON - Servo 180");
        }
        else if (message == "off")
        {
            digitalWrite(LED_PIN, LOW);
            servo.write(0);

            Serial.println("LED OFF - Servo 0");
        }
        else
        {
            Serial.println("Unknown command");
        }
    }
};
