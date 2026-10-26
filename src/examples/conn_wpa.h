#pragma once

#include <Arduino.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include "config.h"

class ConnWPA
{
public:
    void begin();
    void update();

private:

    char ssid[sizeof(WIFI_SSID)] = WIFI_SSID;
    char pass[sizeof(WIFI_PASS)] = WIFI_PASS;
    int status = WL_IDLE_STATUS;

    void printWifiData();
    void printCurrentNet();
    void printMacAddress(byte mac[]);
};