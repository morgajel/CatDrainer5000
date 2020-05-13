#ifndef Laser_h
#define Laser_h

#include "Arduino.h"
#include "Servo.h"
class Laser
{
  public:
    Laser(int lPin, Servo servo);
    void laserOn();
    void laserOff();
    int setMin(int newMax);
    int setMax(int newMin);
    void move(int dest, int stepSize);
    int limitTilt(int tilt);
    int setTilt(int tilt);
    int getState();
    int getTilt();
  public:
    Servo servo;
  private:
    int _lPin;
    int _state;
    int _tiltMin;
    int _tiltMax;
    int _currentTilt;
};

#endif