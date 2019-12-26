
// #include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>
// #include "config.h"

// // Initialize Constants
// const int laserPin = 12;
// const int panPin = 20;
// const int tiltPin = 21;
// const int serverPort = 80;
// const int maxServoAngle = 180;

// // Volatile states
// volatile int laserState = LOW;
// volatile int currentTilt = 0;
// volatile int currentPan = 0;

// // Complex stuff
// WebServer server(serverPort);

// // Prepare the Lasers
// void laserOn(){
//   digitalWrite(laserPin, HIGH);
// }

// void laserOff(){
//   digitalWrite(laserPin, LOW);
// }

// // Lets Serve some Pages
// void handleNotFound() {
//   String message = "File Not Found\n\n";
//   message += "URI: ";
//   message += server.uri();
//   message += "\nMethod: ";
//   message += (server.method() == HTTP_GET) ? "GET" : "POST";
//   message += "\nArguments: ";
//   message += server.args();
//   message += "\n";
//   for (uint8_t i = 0; i < server.args(); i++) {
//     message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//   }
//   server.send(404, "text/plain", message);
// };


// void connectToWifi(void){
//   //Configure WIFI using config.h values
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }


//   Serial.println("");
//   Serial.print("Connected to ");
//   Serial.println(ssid);
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());


//   if (MDNS.begin("esp32")) {
//     Serial.println("MDNS responder started");
//   }
  
// }


// // Setup the initial values.
// void setup(void) {
//   // Set our Serial port rate
//   Serial.begin(115200);
//   // Prep our laser pin
//   pinMode(laserPin, OUTPUT);
//   // Wait for connection
//   connectToWifi();

// // Define all of these handlers for incoming URLs as anonymous lambda functions
//   server.on("/", [](){
//     String message = "Catdrainer 5000; now with more Servos";
//     server.send(200, "text/plain", message);
//   });

//   server.on("/left", []() {
//     server.send(200, "text/plain", "Turn Left");
//   });
  
//   server.on("/laserOn", []() {
//     server.send(200, "text/plain", "Fire da Laser PEW PEW PEW");
//     laserOn();
//   });
  
//   server.on("/laserOn", []() {
//     server.send(200, "text/plain", "*terminator dies*");
//     laserOff();
//   });

//   server.onNotFound(handleNotFound);

//   server.begin();
//   Serial.println("HTTP server started");
// }

// // The big bad loop in the sky
// // Note, the server handleClient does all the heavy lifting.
// void loop(void) {
//   server.handleClient();
// }

void setup(void){
  Serial.begin(115200);
}

void loop(void){
  Serial.println("derp");
}

