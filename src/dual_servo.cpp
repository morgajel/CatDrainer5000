
// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>
// #include <Servo.h>
// #include "config.h"
// #include <ctype.h>


// // Initialize Constants
// const int laserPin = 36;
// const int panPin = 23;
// const int tiltPin = 22;
// const int serverPort = 80;

// // Volatile states
// volatile int laserState = LOW;
// volatile int currentTilt = 0;
// volatile int currentPan = 0;
// volatile int destTilt = 90;
// volatile int destPan = 90;

// int panMin = 50;
// int panMax = 160;
// int tiltMin = 30;
// int tiltMax = 100;


// // Complex stuff
// WebServer server(serverPort);
// Servo pan_servo;
// Servo tilt_servo;

// int move_direction(int current, int target, int step, int min_size, int max_size){
//   int result = target; // default to target.

//   if (target <= min_size){
//     result = min_size;
//   } else if (target >=max_size){
//     result = max_size;  
//   } else if (target < (current - step)){
//     result =  (current - step) ;
//   } else if (target > (current + step)){
//     result = (current + step) ;
//   }
//   return result;
// }
// int limit_axis(int direction, int dmin, int dmax){
//   if (direction <dmin){
//     return dmin;
//   }else if (direction > dmax){
//     return dmax;
//   } else {
//     return direction;
//   }

// }

// void move_laser(int newPan, int newTilt, int step){
//   newPan = limit_axis(newPan, panMin, panMax);
//   newTilt = limit_axis(newTilt, tiltMin, tiltMax);
//   int old_pan = currentPan;
//   int old_tilt = currentTilt;
//   Serial.printf("starting move to (%i, %i) at %i stepsize.\n", newPan, newTilt, step);
//   // If the new location is within our acceptable region
//   while(newPan != currentPan && newTilt!=currentTilt){
//     Serial.printf("(%i, %i) towards (%i, %i)\n", currentPan, currentTilt, newPan, newTilt);

//     int pan_val = move_direction(currentPan, newPan, step, panMin, panMax); // only moves a bit
//     pan_servo.write(pan_val);
//     old_pan = currentPan;
//     currentPan = pan_val;

//     int tilt_val = move_direction(currentTilt, newTilt, step, tiltMin, tiltMax); // only moes a bit
//     tilt_servo.write(tilt_val);
//     old_tilt = currentTilt;
//     currentTilt = tilt_val;
//   }

//   int total_moved = sqrt(pow(abs(old_pan - currentPan),2)+pow(abs(old_tilt-currentTilt),2));
//   delay(total_moved*10);
//   Serial.printf("end_move: %i steps\n", total_moved);

// }

// // Prepare the Lasers
// void laserOn(){
//   digitalWrite(laserPin, HIGH);
//   Serial.println("Powering on laser");
// }

// void laserOff(){
//   digitalWrite(laserPin, LOW);
//   Serial.println("Powering off laser");
// }

// void automate(){
//   laserOn();
//   move_laser(0,0,1);
//   delay(2000);
//   move_laser(0,180,1);
//   delay(2000);
//   move_laser(180,180,1);
//   delay(2000);
//   move_laser(180,0,1);
//   delay(2000);
//   move_laser(0,0,1);
//  delay(2000);
// }

// void connectToWifi(void){
//   //Configure WIFI using config.h values
//   WiFi.mode(WIFI_AP_STA);
//   WiFi.setHostname("CatDrainer5000");
//   delay(2000);
//   Serial.println("Mac Addr:");
//   Serial.println(String(WiFi.macAddress()));
//   WiFi.begin(ssid, password);
//   Serial.println(WiFi.isConnected());
  
//   while ( WiFi.status() != WL_CONNECTED) {
//     delay(500);
    
//     Serial.print(".");
//     Serial.printf("%i" , WiFi.status());
    
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

// void handleMoveArgs(){
//   String message = "";
//   int target_pan = currentPan;
//   int target_tilt = currentTilt;
//   if (server.arg("pan") != "") {
//     target_pan = limit_axis(server.arg("pan").toInt(), panMin, panMax );
//   }
// if (server.arg("tilt") != "") {
//     target_tilt = limit_axis(server.arg("tilt").toInt(), tiltMin, tiltMax );
//   }
//   move_laser(target_pan, target_tilt, 1);
//   server.send(200, "text/html", "OK.");          //Returns the HTTP response

// }


// // Setup the initial values.
// void setup(void) {
//   // Set our Serial port rate
//   Serial.begin(115200);
//   // Prep our laser pin
//   pinMode(laserPin, OUTPUT);
//   digitalWrite(laserPin, LOW);
//   pan_servo.attach(panPin); 
//   tilt_servo.attach(tiltPin);
//   // Wait for connection
//   connectToWifi();

// // Define all of these handlers for incoming URLs as anonymous lambda functions
//   server.on("/", [](){
//       String data = "<!DOCTYPE html> <html>\n";
//     data += "<head>\n";
//     data += "  <style>\n";
//     data += "    .center {margin: auto; font-size: 80px; font-weight: heavy; text-align:center;}\n";
//     data += "  </style>\n";
//     data += "</head>\n";
//     data += "<body>\n";
//     data += "  <div class='center'> \n";
//     data += "    <a href='/forward'>&#9650;</a></div>\n";
//     data += "  <div class='center'> \n";
//     data += "    <a href='/left'>&#9664;</a>\n";
//     data += "    <a href='/stop'>&#128721;</a>\n";
//     data += "    <a href='/right'>&#9654;</a></div>\n";
//     data += "  <div class='center'> \n";
//     data += "    <a href='/backward'>&#9660;</a>\n";
//     data += "  </div>\n";
//     data += "</body></html>";
//     String message = "<!DOCTYPE html>\n";
//     message += "<html>" ;
//     message += "<head>" ;
//     message += "</head>" ;
//     message += "<body>" ;
//     message += "<b>Catdrainer 5000:</b> now with more Servos." ;
//     message += " <button type=\"button\" onlick=\"const userAction = async () => {"
//   "const response = await fetch('/laseroff');"
//   "const myJson = await response.json(); //extract JSON from the http response"
//   "}\">Enable Laser</button>";
//     message += "</body></html>";
//     server.send(200, "text/html", message);
//   });

//   server.on("/automate", []() {
//      server.send(200, "text/plain", "Turn Left");
     
//   });

//   server.on("/laseron", []() {
//     server.send(200, "text/plain", "armed");
//     laserOn();
//   });
//   server.on("/laseroff", []() {
//     server.send(200, "text/plain", "disarmed");
//     laserOff();
//   });

//   server.on("/move", handleMoveArgs);   //Associate the handler function to the path

// //   server.onNotFound(handleNotFound);

//   server.begin();
//   Serial.println("HTTP server started");
//   digitalWrite(laserPin, HIGH);
// }


// // Note, the server handleClient does all the heavy lifting.
// void loop(void) {
//   server.handleClient();
  
  
// }
