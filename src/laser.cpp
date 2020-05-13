#include "Arduino.h"
#include "laser.h"
#include <Servo.h>



Laser::Laser(int lPin, Servo lServo) {
  pinMode(lPin, OUTPUT);
  
  servo = lServo;
  _lPin = lPin;
  _state = LOW;
  _tiltMin = 0;
  _tiltMax = 180;
  _currentTilt = 90;  // Start the neutral position

}

// Prepare the Lasers
void Laser::laserOn(){
  _state = HIGH;
  digitalWrite(_lPin, _state);
  Serial.println("Powering on laser");
}

void Laser::laserOff(){
  _state=LOW;
  digitalWrite(_lPin, _state);
  Serial.println("Powering off laser");
}

int Laser::setMin(int newMin){
  if (newMin < _tiltMax){
    _tiltMin = limitTilt(newMin);
  }
  return _tiltMin;
}

int Laser::setMax(int newMax){
  if (newMax > _tiltMin){
    _tiltMax = limitTilt(newMax);

  } 
  return _tiltMax;
}

int Laser::limitTilt(int tilt){
    return max(0,min(tilt,180));    
}
int Laser::getState(){
    return _state;
}
int Laser::getTilt(){
    return servo.read();
}
int Laser::setTilt(int tilt){
    _currentTilt = limitTilt(tilt);
    Serial.println(_currentTilt);
    servo.write( _currentTilt);
    Serial.printf("Trying to set to %i....now set at %i \n", _currentTilt, servo.read());
    return _currentTilt;
}

void parseAuto(String code){
  // H = High relative Movement (in Degrees towards 180)
  // A = Absolute position (in degrees between tiltMin and tiltMax)
  // L = Low relative Movement (in Degrees towards 0)
  // T = Time to move (in seconds)
  // example code: A90;L10S1;R30T2;R10T1;A10T5;A180T10;
  

}

void Laser::move(int dest, int stepSize){ //stepsize should always be positive
  dest = limitTilt(dest);
  Serial.printf("starting move to ( %i) at %i stepsize.\n", dest, stepSize);

  while(_currentTilt != dest){
      Serial.print("inaloop...");
    int distRem = abs(_currentTilt - dest );
    if (distRem <= stepSize){
        setTilt(dest);
    } else if (_currentTilt > dest){
        setTilt(_currentTilt - stepSize);
    } else if (_currentTilt < dest) {
        setTilt(_currentTilt + stepSize);
    } else {
        Serial.printf("This should never happen:");
        Serial.printf("cur: %i dest: %i step: %i distRem: %i\n", _currentTilt, dest, stepSize, distRem);
        exit(1);
    }
    Serial.println("DEEELAAAAY");
    delay(stepSize*10);
  }

}
