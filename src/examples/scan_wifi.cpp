
#include <SPI.h>

#include <Arduino.h>
#include <WiFiNINA.h>
#include "scan_wifi.h"


void ScanWifi::listNetworks() {

  // scan for nearby networks:

  Serial.println("** Scan Networks **");

  int numSsid = WiFi.scanNetworks();

  if (numSsid == -1) {

    Serial.println("Couldn't get a wifi connection");

    while (true);

  }

  // print the list of networks seen:

  Serial.print("number of available networks:");

  Serial.println(numSsid);

  // print the network number and name for each network found:

  for (int thisNet = 0; thisNet < numSsid; thisNet++) {

    Serial.print(thisNet);

    Serial.print(") ");

    Serial.print(WiFi.SSID(thisNet));

    Serial.print("\tSignal: ");

    Serial.print(WiFi.RSSI(thisNet));

    Serial.print(" dBm");

    Serial.print("\tEncryption: ");

    printEncryptionType(WiFi.encryptionType(thisNet));

  }
}

void ScanWifi::printEncryptionType(int thisType) {

  switch (thisType) {

    case ENC_TYPE_WEP:

      Serial.println("WEP");

      break;

    case ENC_TYPE_TKIP:

      Serial.println("WPA");

      break;

    case ENC_TYPE_CCMP:

      Serial.println("WPA2");

      break;

    case ENC_TYPE_NONE:

      Serial.println("None");

      break;

    case ENC_TYPE_AUTO:

      Serial.println("Auto");

      break;

    case ENC_TYPE_UNKNOWN:

    default:

      Serial.println("Unknown");

      break;

  }
}

void ScanWifi::printMacAddress(byte mac[]) {

  for (int i = 5; i >= 0; i--) {

    if (mac[i] < 16) {

      Serial.print("0");

    }

    Serial.print(mac[i], HEX);

    if (i > 0) {

      Serial.print(":");

    }

  }

  Serial.println();
}

void ScanWifi::begin() {

  Serial.begin(9600);

  while (!Serial) {

  }


  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }

  // print your MAC address:

  byte mac[6];

  WiFi.macAddress(mac);

  Serial.print("MAC: ");

  printMacAddress(mac);
}

void ScanWifi::update() {

  // scan for existing networks:

  Serial.println("Scanning available networks...");

  listNetworks();

  delay(10000);
}
