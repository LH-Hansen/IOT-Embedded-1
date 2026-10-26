#include <SPI.h>
#include <WiFiNINA.h>

#include "examples/scan_wifi.h"
#include "examples/conn_wpa.h"
#include "examples/ssl_client.h"
#include "examples/web_server.h"

ScanWifi scan_wifi;
ConnWPA conn_wpa;
SSLClient ssl_client;
WebServer web_server;

void setup() {
  // scan_wifi.begin();
  // conn_wpa.begin();
  // web_server.begin();
    ssl_client.begin();
}

void loop() {
  // scan_wifi.update();
  // conn_wpa.update();
  // web_server.update();
    ssl_client.update();
}
