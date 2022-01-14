#pragma once
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <RH_ASK.h>
#include <receiver.h>

extern ESP8266WebServer server;  
extern RH_ASK driver;
extern unsigned long UTC0;
extern receiver receivers[];