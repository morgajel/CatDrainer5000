#include "Arduino.h"
#include "laser.h"
#include <Servo.h>
#include <Regexp.h>


Laser::Laser(int lPin, Servo lServo) {
  pinMode(lPin, OUTPUT);
  servo = lServo;
  _lPin = lPin;
  _state = LOW;
  _tiltMin = 700;
  _tiltMax = 2300;
  _currentTilt = 1500;  // Start the neutral position
  _remaining_moves = 0;
  digitalWrite(lPin, _state);
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
    return max(_tiltMin,min(tilt,_tiltMax));
}
int Laser::getState(){
    return _state;
}
int Laser::getRemainingMoves(){
    return _remaining_moves;
}
int Laser::getTilt(){
    return servo.read();
}
int Laser::setTilt(int tilt){
    _currentTilt = limitTilt(tilt);
    servo.writeMicroseconds( _currentTilt);
    //Serial.printf("Trying to set to %i....now set at %i \n", _currentTilt, servo.read());
    return _currentTilt;
}
void Laser::stutters(int destination, int speed, int stutterCount){
  // If  current=0, distance= 100, stutter=10 and speed=5000
  // stutter_distance= (100-0)/10 = 10 degrees
  // stutter_speed = 5000/10 = 500ms

  //Serial.println("  stuttering function destination: " + String(destination) +" speed: " + String(speed)+"ms stutters:"+ String(stutterCount));
  int stutter_distance = (destination - _currentTilt)/stutterCount; // Distance to travel per stutter
  int stutter_speed = speed/stutterCount;

  for (int i = 0; i < stutterCount; i++){ // for each stutter
    int next_destination = _currentTilt+stutter_distance;
    Laser::move( next_destination, stutter_speed);
    delay(stutter_speed);
  }
}

void Laser::test(){

  // Serial.println("==== Move to zero ====");
  // Laser::move(0, 1000);
  // delay(3000);
  // Serial.println("==== Move to 180 in 1 second ====");
  // Laser::move(180, 1000);
  // delay(3000);

  // Serial.println("==== Move to 0 in 4 second ====");
  // Laser::move(0, 4000);
  // delay(3000);
  // Serial.println("==== Move to 180 in 4 second ====");
  // Laser::move(180, 4000);
  // delay(3000);

  Serial.println("==== Move to 0 in 2 second and 1 stutter ====");
  Laser::stutters(0,2000,1);
  Serial.println("==== Move to 180 in 2 seconds and 1 stutter ====");
  Laser::stutters(180,2000,1); // destination, speed, stuttercount

  Serial.println("==== Move to 0 in 2 seconds and 2 stutters ====");
  Laser::stutters(0,2000,2);
  Serial.println("==== Move to 180 in 1 second and 2 stutters ====");
  Laser::stutters(180,2000,2); // destination, speed, stuttercount

  Serial.println("==== Move to 0 in 8 seconds and 1 stutter ====");
  Laser::stutters(0,8000,1);
  Serial.println("==== Move to 180 in 8 seconds and 1 stutter ====");
  Laser::stutters(180,8000,1);

  Serial.println("==== Move to 0 in 8 seconds and 10 stutters ====");
  Laser::stutters(0,8000,10);
  Serial.println("==== Move to 180 in 8 seconds and 10 stutters ====");
  Laser::stutters(180,8000,10);

  // Laser::move(0,0.5);
  // Laser::move(180,0.5);
  // Laser::move(0,1);
  // Laser::move(180,1);
  // Laser::move(0,2);
  // Laser::move(180,2);
  // Laser::move(0,10);
  // Laser::move(180,10);
  // Laser::move(0,10);
  // Laser::move(5,10);

}
void Laser::automate(int moves){
  Serial.println("automating...");
  for (_remaining_moves = moves; _remaining_moves > 0 ; _remaining_moves--){
    int destination = random(_tiltMax - _tiltMin) + _tiltMin;
    int speed = random(500,10000);
    int stutterCount = random(2, 10);
    int behavior = random(1,3);
    switch (behavior) {
      case 1: // smooth
        {
          Serial.println("Move behavior");
          Laser::move(destination, speed);
          break;
        }
      case 2: // stuttered
        {
          Serial.println("Stutter behavior");
          Laser::stutters(destination, speed, stutterCount);
          break;
        }
      // case 3: // zigzag
      // int zigzags = random(1,destination/10);
      //   while (zigzags >0){
      //     int zigdistance = x;
      //     move(destination/stutters, speed/stutters);
      //     delay(speed/stutters);
      //     zigzags-=1;
      //   break;
      default: //silent
        {
          Serial.println("taking a moment...");
          delay(speed/2);
          break;
        }
    }

  }
}

void Laser::move(int destination, int speed){ // destination = absolute destination, speed=milliseconds
  destination = limitTilt(destination);       // make sure it's in our travel range
  int distance = destination - _currentTilt;  // Distance to travel in degrees
  // Serial.println("    ++++++++");
  Serial.printf("    starting move from %i -> %i = %i degrees in %i ms.\n", _currentTilt, destination, distance, speed);
  // if distance=100 degrees and speed=10 seconds,
  // or for every .1 second move 1 destination.

  for (int i=0 ; i < abs(distance); i++ ){
    // Serial.printf("      Move step %i of %i. current: %i, adding %i in %i ms\n",
    //               i, abs(distance), _currentTilt, (distance/abs(distance)), abs(speed/distance) );
    setTilt(_currentTilt + (distance/abs(distance)));
    delay(abs(speed/distance));
  }
}

