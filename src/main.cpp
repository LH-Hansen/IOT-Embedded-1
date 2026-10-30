#include "examples/scan_wifi.h"
#include "examples/conn_wpa.h"
#include "examples/ssl_client.h"
#include "examples/web_server.h"
#include "mqtt/wifi_simple_rec.h"
#include "mqtt/subscriber.h"
#include "mqtt/hive.h"
#include "mqtt/influxdb.h"

ScanWifi scan_wifi;
ConnWPA conn_wpa;
SSLClient ssl_client;
WebServer web_server;
WifiSimpleRec wifi_simple_rec;
Subscriber subscriber;
HiveLocal hive;
InfluxDB influxdb;

void setup() {
  // scan_wifi.begin();
  // conn_wpa.begin();
  // web_server.begin();
  // ssl_client.begin();
  //wifi_simple_rec.begin();
  // subscriber.begin();
  // hive.begin();
  influxdb.begin();
}

void loop() {
  // scan_wifi.update();
  // conn_wpa.update();
  // web_server.update();
  // ssl_client.update();
  //wifi_simple_rec.update();
  // subscriber.update();
  // hive.update();
  influxdb.update();
}
