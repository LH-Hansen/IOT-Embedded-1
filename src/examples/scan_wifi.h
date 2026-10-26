#pragma once

#include <Arduino.h>

class ScanWifi
{
public:
    void begin();
    void update();

private:
    void listNetworks();
    void printEncryptionType(int thisType);
    void printMacAddress(byte mac[]);
};