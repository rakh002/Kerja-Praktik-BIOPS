#include <ESPmDNS.h>
#include <AsyncElegantOTA.h>
#include <elegantWebpage.h>
#include <Hash.h>
#include <DNSServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <MQTT.h>
#include <MQTTClient.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TFPeriodic.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

int data;
bool wifiConnected = false;

// Variabel yg diinginkan
int currentValue;
int previousValue;
int maxValue;
int minValue;

bool checkData = false;
bool wifiScan = false;

// Search for parameter in HTTP POST request
const char *PARAM_INPUT_1 = "ssid";
const char *PARAM_INPUT_2 = "pass";

// Variables to save values from HTML form
String ssid;
String pass;

// File paths to save input values permanently
const char *ssidPath = "/ssid.txt";
const char *passPath = "/pass.txt";

//=======================Setup LED=======================//
const int LED1 = 32;
const int LED2 = 33;

void setupLED()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}
