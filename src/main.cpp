#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <ctype.h>
#include "config.h"
#include "ota.h"
#include "html_index.h"
#include "laser.h"

// Ilnitialize Constants
const int laserPin = GPIO_NUM_16; // The laser connects to this pin and ground
const int tiltPin = GPIO_NUM_5; // The data line of the servo connects here
const char* hostname = "Catdrainer2500";
const int serverPort = 80; //sorry, just simple HTTP.

// Complex stuff
WebServer server(serverPort);
Servo servo;
String url;
Laser laser(laserPin, servo);

//======================================================
void connectToWifi(void){
  //Configure WIFI using config.h values
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname(hostname);
  delay(2000);
  Serial.println("Mac Addr: " + (String)WiFi.macAddress());
  WiFi.begin(ssid, passphrase);

  Serial.print("Attempting WiFi connection");
  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("  Connected to " + (String)ssid);
  Serial.println("IP address: " + WiFi.localIP().toString());
  url = "http://" + WiFi.localIP().toString() + "/"  ;
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started.");
  }
}

//======================================================
void(* resetFunc) (void) = 0; //declare reset function @ address 0
//  ESP.restart()  TODO this might be a better solution.

void handleNotFound(){
  server.send(404, "text/html", "that's not right.");          //Returns the HTTP response
}

void handleSetTilt(){
  String message = "";
  int tilt = server.arg("tilt").toInt();
  laser.setTilt(tilt);
  server.send(200, "text/html", "tilted to " + (String)tilt);          //Returns the HTTP response
}

void handleAutomation(){
  int moves = server.arg("moves").toInt();
  server.send(200, "text/plain", "beep boop");
  laser.laserOn();
  laser.automate(moves);
  laser.laserOff();
}

void handleStatusRequest(){
  if (laser.getRemainingMoves()>0){
    server.send(206, "text/json", "{\"Remaining Moves\": " + (String)laser.getRemainingMoves() + "}");
  } else {
    server.send(200, "text/json", "{\"Remaining Moves\": " + (String)laser.getRemainingMoves()+ "}");
  }
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
//                                Setup                                     //
//##########################################################################//
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
  ota_setup(hostname, password);

//==========================================================
// Define all of these handlers for incoming URLs as anonymous lambda functions
  server.on("/", [](){
    //server.send(200, "text/html", indexPage);
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", indexPage);
  });

  server.on("/toggleLaser", []() {
    server.sendHeader("Connection", "close");
    if (laser.getState() == LOW) {
      server.send(200, "text/plain", "Laser armed");
      laser.laserOn();
    } else {
      server.send(200, "text/plain", "Laser disarmed");
      laser.laserOff();
    }
  });

  server.on("/test", []() {
    laser.laserOn();
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", "testing 123...");
    laser.test();
  });
  server.on("/automate", handleAutomation); 
  server.on("/setTilt", handleSetTilt);   //Associate the handler function to the path
  server.on("/status", handleStatusRequest);
  server.on("/reset", resetFunc);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  laser.setMax(160);
  laser.setMin(30);
}

// Note, the server handleClient does all the heavy lifting.
void loop(void) {
  server.handleClient();
}
