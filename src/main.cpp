#include <RH_ASK.h>
#include <Arduino.h>
#include <SPI.h> // Not actually used but needed to compile
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

//WiFi-Settings
#define WLAN_SURFACE_HOTSPOT
#include "settings.h"

//Webserver-settings
ESP8266WebServer server(80);  
int timeshift = 0;
String Argument_Name, Clients_Response;

//Time-Settings
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnestag", "Freitag", "Sammstag"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//forward delaration
void setupDisplay();
void HandleClient();
void ShowClientResponse();
void setDisplay(String& time);

RH_ASK driver(2000, D1, D3);

void setup()
{
  pinMode(D3, OUTPUT);
  Serial.begin(74880);	  // Debugging only

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

  server.begin(); Serial.println("Webserver started..."); // Start the webserver
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Use this URL to connect: http://");// Print the IP address
  Serial.print(WiFi.localIP());Serial.println("/");

  //Setup Webserver
  // Next define what the server should do when a client connects
  server.on("/", HandleClient); // The client connected with no arguments e.g. http:192.160.0.40/
  //server.on("/result", ShowClientResponse);
  Serial.println("Server listening");
}

void HandleClient() {
  String webpage;
  webpage =  "<html>";
   webpage += "<head><title>ESP8266 Input Example</title>";
    webpage += "<style>";
     webpage += "body { background-color: #E6E6FA; font-family: Arial, Helvetica, Sans-Serif; Color: blue;}";
    webpage += "</style>";
   webpage += "</head>";
   webpage += "<body>";
     String IPaddress = WiFi.localIP().toString();
     webpage += "<form action='http://"+IPaddress+"' method='POST'>";
      webpage += "<select name='clock1'>";
        webpage += "<option value='0'>UTC 0</option>";
        webpage += "<option value='3600'>UTC +1</option>";
        webpage += "<option value='10800'>UTC +3</option>";
        webpage += "<option value='-18000'>UTC -5</option>";
      webpage += "</select>";
      webpage += "<input type='submit' value='Apply Change'>";
     webpage += "</form>";
   webpage += "</body>";
  webpage += "</html>";
  server.send(200, "text/html", webpage); // Send a response to the client asking for input
  
  // Arguments were received
  if (server.args() > 0 ) { 
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      Argument_Name = server.argName(i);
      if (server.argName(i) == "clock1") {
        Serial.print(" Input received was: ");
        Serial.println(server.arg(i));
        Clients_Response = server.arg(i);
        timeshift = server.arg(i).toInt();
      }
    }
  }
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
  //Serial.println(time);

  //Output-Data
  String output = timeClient.getFormattedTime();  
  setDisplay(output);

  String stime = String(time);
  const char *msg = stime.c_str();
  //const char *msg = "1233";
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