#include <Arduino.h>
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Servo.h>
#include <BlynkSimpleEsp32.h>

#include "config.h"


// Initialize Constants
const int laserPin = 11;
const int panPin = 23;
const int tiltPin = 22;

// Volatile states
volatile int laserState = LOW;
volatile int currentTilt = 90;
volatile int currentPan = 90;
volatile int destTilt = 90;
volatile int destPan = 90;
volatile int stepSpeed = 10; // This is 10 ms/degree rotation


Servo panServo;
Servo tiltServo;

BLYNK_WRITE(V2) {
  int x = param.asInt();
  digitalWrite(laserPin, x);
  digitalWrite(laserPin, LOW);
  Serial.println("Set Pin " + String(x));
}

BLYNK_WRITE(V3) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  panServo.write(x);
  tiltServo.write(y);
}
BLYNK_WRITE(V4) {
  int y = param.asInt();
  tiltServo.write(y);
}



// // Prepare the Lasers
void laserOn(){
   digitalWrite(laserPin, HIGH);
}

void laserOff(){
   digitalWrite(laserPin, LOW);
}



void connectToWifi(void){

  Blynk.config(token);
  Blynk.begin(token, ssid, password);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  
}

// Setup the initial values.
void setup(void) {
  // Set our Serial port rate
  Serial.begin(115200);
  // Prep our laser pin
  pinMode(laserPin, OUTPUT);



  panServo.attach(panPin);
  tiltServo.attach(tiltPin);

  // Wait for connection
  Serial.println("Let's print some dots!");
  connectToWifi();

}


// The big bad loop in the sky
// Note, the server handleClient does all the heavy lifting.
void loop(void) {
  Blynk.run();
}
