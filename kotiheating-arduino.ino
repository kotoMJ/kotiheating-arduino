#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <TaskScheduler.h>
#include "privateConfig.h"

#define TEMP_INVALID 850

#define PIN_RELAY D6
#define PIN_BUTTON1 D3
#define PIN_BUTTON2 D7



Scheduler scheduler;

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire ds(D5);

