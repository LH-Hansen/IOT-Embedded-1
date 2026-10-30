
#pragma once

#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <DHT.h>
#include <Servo.h>
#include "config.h"

class InfluxDB
{
public:
    void begin()
    {
        Serial.begin(115200);

        dht.begin();

        servo.attach(SERVO_PIN);
        servo.write(0);

        // Connect to WiFi
        WiFi.begin(ssid, wifiPassword);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        Serial.println("\nWiFi connected");

        // Register MQTT callback
        instance = this;
        mqttClient.setUsernamePassword(mqttUser, mqttPassword);
        mqttClient.onMessage(messageCallback);

        connectMqtt();
    }

    void update()
    {
        if (WiFi.status() != WL_CONNECTED)
            return;

        if (!mqttClient.connected())
            connectMqtt();

        mqttClient.poll();

        // Publish telemetry every 5 seconds
        if (millis() - lastSend >= 5000)
        {
            lastSend = millis();

            float temperature = dht.readTemperature();
            float humidity = dht.readHumidity();

            if (isnan(temperature) || isnan(humidity))
            {
                Serial.println("DHT reading failed");
                return;
            }

            String json = "{\"Temperature\":" +
                          String(temperature, 1) +
                          ",\"Humidity\":" +
                          String(humidity, 1) + "}";

            mqttClient.beginMessage(telemetryTopic);
            mqttClient.print(json);
            mqttClient.endMessage();

            Serial.println("Sent: " + json);
        }
    }

private:
    unsigned long lastSend = 0;

    const char *ssid = WIFI_SSID;
    const char *wifiPassword = WIFI_PASS;

    const char *broker = BROKER;
    const int port = 1883;

    const char *mqttUser = USERNAME;
    const char *mqttPassword = PASSWORD;

    const char *telemetryTopic = "school/demo/telemetry";
    const char *servoTopic = "school/demo/servo";

    WiFiClient wifiClient;
    MqttClient mqttClient{wifiClient};

    DHT dht{DHT_PIN, DHT_TYPE};
    Servo servo;

    // MQTT callback needs a static function
    static InfluxDB *instance;

    static void messageCallback(int messageSize)
    {
        if (instance != nullptr)
            instance->onMessage(messageSize);
    }

    void onMessage(int messageSize)
    {
        String message;

        String topic = mqttClient.messageTopic();

        while (mqttClient.available())
        {
            message += (char)mqttClient.read();
        }

        Serial.println("Topic: " + topic);
        Serial.println("Received: " + message);

        if (topic == servoTopic)
        {
            if (message == "on")
            {
                servo.write(90);
                Serial.println("Servo ON");
            }
            else if (message == "off")
            {
                servo.write(0);
                Serial.println("Servo OFF");
            }
        }
    }

    void connectMqtt()
    {
        while (!mqttClient.connect(broker, port))
        {
            Serial.print("MQTT failed: ");
            Serial.println(mqttClient.connectError());
            delay(3000);
        }

        mqttClient.subscribe(servoTopic);

        Serial.println("MQTT connected");
        Serial.println("Subscribed to school/demo/servo");
    }
};

// Define the static instance (C++11 compatible)
InfluxDB *InfluxDB::instance = nullptr;
