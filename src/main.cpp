#include <RH_ASK.h>
#include <Arduino.h>
#include <SPI.h> // Not actually used but needed to compile
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <receiver.h>

//WiFi-Settings
#define WLAN_SURFACE_HOTSPOT
#include <settings.h>

//Webserver-settings
#include <vars.h>

//Time-Settings
const long utcOffsetInSeconds = 0;
char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnestag", "Freitag", "Sammstag"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//forward delaration
void setupDisplay();
void setupWebserver();
void setDisplay(String& time);

RH_ASK driver(2000, D1, D3);
const unsigned int RECVEIVERS_LENGHT = 3;
receiver receivers[RECVEIVERS_LENGHT]={};
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
  
  //register receiver
  receivers[0] = receiver(1);
  receivers[1] = receiver(2);
  receivers[2] = receiver(3);
}
unsigned long UTC0=0;

void loop()
{
  server.handleClient();

  //Get Time 
  timeClient.update();
  unsigned long sec=timeClient.getSeconds();
  unsigned long min=timeClient.getMinutes();
  unsigned long hor=timeClient.getHours();
  UTC0 = sec + min*60UL + hor*3600UL;

  //Output-Data
  String output = timeClient.getFormattedTime();  
  setDisplay(output);

  //Update all receivers
  for(unsigned int index = 0; index < RECVEIVERS_LENGHT; index++){
    receiver rec = receivers[index];
    rec.notifyUpdate();
  }
  delay(100);
}