#pragma once

#include <WiFiNINA.h>
#include <Arduino.h>
#include <SPI.h>
#include "config.h"

class SSLClient
{
public:
    void begin();
    void update();

private:
    char ssid[sizeof(WIFI_SSID)] = WIFI_SSID;
    char pass[sizeof(WIFI_PASS)] = WIFI_PASS;
    int status = WL_IDLE_STATUS;
    int keyIndex = 0;  
    char server[sizeof("www.google.com")] = "www.google.com";

    WiFiSSLClient client;

    void printWiFiStatus()
};