
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Servo.h>
#include "config.h"


// Initialize Constants
const int laserPin = 11;
const int panPin = 23;
const int tiltPin = 22;
const int serverPort = 80;

// Volatile states
volatile int laserState = LOW;
volatile int currentTilt = 0;
volatile int currentPan = 0;
volatile int destTilt = 90;
volatile int destPan = 90;

int panMin = 50;
int panMax = 160;
int tiltMin = 30;
int tiltMax = 100;


// Complex stuff
WebServer server(serverPort);
Servo pan_servo;
Servo tilt_servo;

int move_direction(int current, int target, int step, int min_size, int max_size){
  int result = target; // default to target.

  if (target <= min_size){
    result = min_size;
  } else if (target >=max_size){
    result = max_size;  
  } else if (target < (current - step)){
    result =  (current - step) ;
  } else if (target > (current + step)){
    result = (current + step) ;
  }
  return result;
}

void move_laser(int newPan, int newTilt, int step){

  // If the new location is within our acceptable region
  int pan_val = move_direction(currentPan, newPan, step, panMin, panMax);
  pan_servo.write(pan_val);
  currentPan = currentPan;
  int tilt_val = move_direction(currentTilt, newTilt, step, tiltMin, tiltMax);
  tilt_servo.write(tilt_val);
  currentTilt = tilt_val;
  delay(2000);

}

// Prepare the Lasers
void laserOn(){
  digitalWrite(laserPin, HIGH);
}

void laserOff(){
  digitalWrite(laserPin, LOW);
}

void automate(){
  laserOn();
  move_laser(0,0,1);
  delay(2000);
  move_laser(180,180,1);
  laserOff();

}

void connectToWifi(void){
  //Configure WIFI using config.h values
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname("CatDrainer5000");
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
  pan_servo.attach(panPin); 
  tilt_servo.attach(tiltPin);
  // Wait for connection
  connectToWifi();

// Define all of these handlers for incoming URLs as anonymous lambda functions
  server.on("/", [](){
      String data = "<!DOCTYPE html> <html>\n";
    data += "<head>\n";
    data += "  <style>\n";
    data += "    .center {margin: auto; font-size: 80px; font-weight: heavy; text-align:center;}\n";
    data += "  </style>\n";
    data += "</head>\n";
    data += "<body>\n";
    data += "  <div class='center'> \n";
    data += "    <a href='/forward'>&#9650;</a></div>\n";
    data += "  <div class='center'> \n";
    data += "    <a href='/left'>&#9664;</a>\n";
    data += "    <a href='/stop'>&#128721;</a>\n";
    data += "    <a href='/right'>&#9654;</a></div>\n";
    data += "  <div class='center'> \n";
    data += "    <a href='/backward'>&#9660;</a>\n";
    data += "  </div>\n";
    data += "</body></html>";
    String message = "<!DOCTYPE html>\n";
    message += "<html>" ;
    message += "<head>" ;
    message += "</head>" ;
    message += "<body>" ;
    message += "<b>Catdrainer 5000:</b> now with more Servos." ;
    message += "</body></html>";
    server.send(200, "text/html", message);
  });

  server.on("/automate", []() {
     server.send(200, "text/plain", "Turn Left");
     
  });
  
//   server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(1000);
  automate();
  Serial.println("oof");
}


// Note, the server handleClient does all the heavy lifting.
void loop(void) {
  server.handleClient();
  
  
}
