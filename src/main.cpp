#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <ctype.h>
#include "config.h"
#include "html_index.h"
#include "laser.h"


// Initialize Constants
const int laserPin = 32; // The laser connects to this pin and ground
const int tiltPin = 25; // The data line of the servo connects here
const char* hostname = "Catdrainer2500";
const int serverPort = 80; //sorry, just simple HTTP.

// Complex stuff
WebServer server(serverPort);
Servo servo;
String url;
Laser laser(laserPin, servo);


void automated(){
  laser.laserOn();
  laser.move(0,1);
  delay(2000);
  laser.move(180,1);
  laser.move(0,1);
  delay(500);
  laser.move(180,2);
  laser.move(0,2);
  delay(500);
  laser.move(180,10);
  laser.move(0,10);
  laser.laserOff();
}

void connectToWifi(void){
  //Configure WIFI using config.h values
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname(hostname);
  delay(2000);
  Serial.println("Mac Addr:");
  Serial.println(String(WiFi.macAddress()));
  WiFi.begin(ssid, password);
  Serial.println(WiFi.isConnected());
  
  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    Serial.print(".");
    Serial.printf("%i" , WiFi.status());
    
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  url = "http://" + String(WiFi.localIP()) + "/"  ;
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  
}



void handleRequest(){
  // when /move is called, this method moves the laser.
  String message = "";
  int tilt = laser.getTilt();
  int step = 10;
  if (server.arg("tilt") != "") {
    tilt = server.arg("tilt").toInt();
  }
  if (server.arg("step") != "") {
    step = server.arg("step").toInt();
  }
  laser.move(tilt,step);
  server.send(200, "text/html", "OK.");          //Returns the HTTP response
}

//##########################################################################//
//                             Setup
void setup(void) {
  // Set our Serial port rate
  Serial.begin(115200);
  // Prep our laser pin
  servo.attach(tiltPin);
  laser.servo = servo;
  // read tiltMax from storage, make sure it's <180
  int tiltMax = 180;
  EEPROM.get(0,tiltMax);
  laser.setMax(tiltMax);
  // read tiltMin from storage, make sure it's > 0
  int tiltMin = 0;
  EEPROM.get(0,tiltMin);
  laser.setMin(tiltMin);
  delay(1000);
  laser.servo.write(30);
  // Wait for connection
  connectToWifi();

// Define all of these handlers for incoming URLs as anonymous lambda functions
  server.on("/", [](){
    server.send(200, "text/html", indexPage);
  });

  server.on("/automated", []() {
     server.send(200, "text/plain", "Beep Boop, running automatically.");
     automated();
  });

  server.on("/toggleLaser", []() {
    if (laser.getState() == LOW) {
      server.send(200, "text/plain", "armed");
      laser.laserOn();
    } else {
      server.send(200, "text/plain", "disarmed");
      laser.laserOff();
    }
  });

  server.on("/move", handleRequest);   //Associate the handler function to the path

//   server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(laserPin, HIGH);
}


// Note, the server handleClient does all the heavy lifting.
void loop(void) {
  server.handleClient();  
}
