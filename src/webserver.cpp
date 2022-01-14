#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <vars.h>

String Argument_Name, Clients_Response;

//init external vars
int timeshift = 0;
ESP8266WebServer server(80);

void HandleClient();

void setupWebserver(){
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