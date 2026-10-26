#pragma once

#include <Arduino.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include "config.h"

class WebServer
{
public:
    void begin();
    void update();

private:
    char ssid[sizeof(WIFI_SSID)] = WIFI_SSID;
    char pass[sizeof(WIFI_PASS)] = WIFI_PASS;
    int keyIndex = 0;
    int status = WL_IDLE_STATUS;

    WiFiServer server{80};

    void printWifiStatus();
};