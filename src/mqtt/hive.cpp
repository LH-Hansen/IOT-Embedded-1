#include "hive.h"

void HiveLocal::begin()
{
    Serial.begin(9600);
    while (!Serial && millis() < 3000)
        ;

    dht.begin();

    connectWiFi();

    mqttClient.setId("MKR1010_DHT_01");
    connectMQTT();
}

void HiveLocal::update()
{
    if (WiFi.status() != WL_CONNECTED)
        connectWiFi();

    if (!mqttClient.connected())
        connectMQTT();

    mqttClient.poll();

    if (millis() - previousMillis >= 2500)
    {
        previousMillis = millis();

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (isnan(temperature) || isnan(humidity))
        {
            Serial.println("DHT read failed");
            return;
        }

        mqttClient.beginMessage("home/temp");
        mqttClient.print(temperature, 1);
        mqttClient.endMessage();

        mqttClient.beginMessage("home/humidity");
        mqttClient.print(humidity, 1);
        mqttClient.endMessage();

        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" C | Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
}

void HiveLocal::connectWiFi()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi...");
        WiFi.begin(ssid, pass);
        delay(3000);
    }

    Serial.println("WiFi connected!");

    Serial.print("Arduino IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    Serial.print("Testing HiveMQ TCP: ");

    WiFiClient testClient;

    if (testClient.connect(broker, port))
    {
        Serial.println("SUCCESS");
        testClient.stop();
    }
    else
    {
        Serial.println("FAILED");
    }
}

void HiveLocal::connectMQTT()
{
    while (!mqttClient.connect(broker, port))
    {
        Serial.print("MQTT error: ");
        Serial.println(mqttClient.connectError());
        delay(3000);
    }

    Serial.println("Connected to HiveMQ!");
}