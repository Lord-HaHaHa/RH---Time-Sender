#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <vars.h>

String Argument_Name, Clients_Response;

ESP8266WebServer server(80);

void HandleClient();

void setupWebserver(){
  server.begin(); // Start the webserver
  Serial.println("Webserver started..."); 
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Use this URL to connect: http://");// Print the IP address
  Serial.print(WiFi.localIP());Serial.println("/");

  //Setup Webserver
  server.on("/", HandleClient);
  Serial.println("Webserver setup complete");
}

void HandleClient() {
  String webpage;
  webpage =  "<html>";
   webpage += "<head><title>Config Timezones</title>";
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
     webpage += "<form action='http://"+IPaddress+"' method='POST'>";
      webpage += "<select name='clock2'>";
        webpage += "<option value='0'>UTC 0</option>";
        webpage += "<option value='3600'>UTC +1</option>";
        webpage += "<option value='10800'>UTC +3</option>";
        webpage += "<option value='-18000'>UTC -5</option>";
      webpage += "</select>";
      webpage += "<input type='submit' value='Apply Change'>";
     webpage += "</form>";
     webpage += "<form action='http://"+IPaddress+"' method='POST'>";
      webpage += "<select name='clock3'>";
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
      if (Argument_Name == "clock1") {
        Serial.print(" Input received was: ");
        Serial.println(server.arg(i));
        Clients_Response = server.arg(i);
        int timeshift = Clients_Response.toInt();
        receivers[0].timeshift = timeshift;
      }
      if (Argument_Name == "clock2") {
        Serial.print(" Input received was: ");
        Serial.println(server.arg(i));
        Clients_Response = server.arg(i);
        int timeshift = Clients_Response.toInt();
        receivers[1].timeshift = timeshift;
      }
      if (Argument_Name == "clock3") {
        Serial.print(" Input received was: ");
        Serial.println(server.arg(i));
        Clients_Response = server.arg(i);
        int timeshift = Clients_Response.toInt();
        receivers[2].timeshift = timeshift;
      }
    }
  }
}