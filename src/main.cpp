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

void(* resetFunc) (void) = 0; //declare reset function @ address 0
//  ESP.restart()  TODO this might be a better solution.

void handleRequest(){
  // when /move is called, this method moves the laser.
  String message = "";
  if (server.arg("tilt") != "" || server.arg("step") !=""){
    int tilt = laser.getTilt();
    int step = 10;
    if (server.arg("tilt") != "") {
      tilt = server.arg("tilt").toInt();
    }
    if (server.arg("step") != "") {
      step = server.arg("step").toInt();
    }
    laser.move(tilt,step);
  }
  server.send(200, "text/html", "OK.");          //Returns the HTTP response
}

void handleMoves(){
String message = "";
int moves = server.arg("moves").toInt();
laser.automate(moves);
server.send(200, "text/html", "OK.");          //Returns the HTTP response
}

void initTiltRange(){
  // read tiltMax from storage, make sure it's <180
  int tiltMax = 180;
  EEPROM.get(0,tiltMax);
  laser.setMax(tiltMax);
  // read tiltMin from storage, make sure it's > 0
  int tiltMin = 0;
  EEPROM.get(0,tiltMin);
  laser.setMin(tiltMin);

}

//##########################################################################//
//                             Setup
void setup(void) {
  // Set our Serial port rate
  Serial.begin(115200);
  // Prep our laser pin
  randomSeed(analogRead(0));
  servo.attach(tiltPin);
  laser.servo = servo;
  initTiltRange();
  // Wait for connection
  connectToWifi();

// Define all of these handlers for incoming URLs as anonymous lambda functions
//=============================   /   =============================
  server.on("/", [](){
    //server.send(200, "text/html", indexPage);
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", indexPage);
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
  server.on("/automate", []() {
    laser.laserOn();
    laser.automate(200);
    laser.laserOff();
  });
  server.on("/test", []() {
    laser.laserOn();
    server.send(200, "text/plain", "testing 123...");
    laser.test();

  });
  //server.on("/automate", handleRequest);   //Associate the handler function to the path
  server.on("/move", handleMoves);   //Associate the handler function to the path
  server.on("/reset", resetFunc);

//   server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  laser.setMax(160);
  laser.setMin(30);
}

// Note, the server handleClient does all the heavy lifting.
void loop(void) {
  server.handleClient();
}
