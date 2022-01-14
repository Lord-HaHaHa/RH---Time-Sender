#include <RH_ASK.h>
#include <Arduino.h>
#include <SPI.h> // Not actually used but needed to compile
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

//WiFi-Settings
#define WLAN_SURFACE_HOTSPOT
#include "settings.h"

//Webserver-settings
#include "vars.h"

//Time-Settings
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnestag", "Freitag", "Sammstag"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//forward delaration
void setupDisplay();
void setupWebserver();
void setDisplay(String& time);

RH_ASK driver(2000, D1, D3);

void setup()
{
  pinMode(D3, OUTPUT);
  Serial.begin(74880);

  setupDisplay();

  //Setup RH
  if (!driver.init())
        Serial.println("init failed");

  //Connect to WiFi
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(WLAN_SSID, WLAN_KEY);
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }
  setupWebserver();
  
}

void loop()
{
  server.handleClient();

  //Get Time 
  timeClient.update();
  unsigned long sec=timeClient.getSeconds();
  unsigned long min=timeClient.getMinutes();
  unsigned long hor=timeClient.getHours();
  unsigned long time = sec + min*60UL + hor*3600UL + timeshift;

  //Output-Data
  String output = timeClient.getFormattedTime();  
  setDisplay(output);

  String stime = String(time);
  const char *msg = stime.c_str();
  if(driver.send((uint8_t *)msg, strlen(msg) + 1)){
    //Serial.print("Transmitted: ");
	//Serial.print(msg);
	//Serial.print(" Lenght: ");
	//Serial.println(strlen(msg));
  }
  else
    Serial.println("Failed to Send");
  driver.waitPacketSent();

}