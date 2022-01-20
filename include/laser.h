#ifndef Laser_h
#define Laser_h

#include <Arduino.h>
#include <Servo.h>
class Laser
{
  public:
    Laser(int lPin, Servo servo);
    void laserOn();
    void laserOff();
    int setMin(int newMax);
    int setMax(int newMin);
    void move(int destination, int speed);
    int limitTilt(int tilt);
    int setTilt(int tilt);
    int getState();
    int getTilt();
    int getRemainingMoves();
    void test();
    void stutters(int degree, int speed, int stutterCount);
    void automate(int moves);
  public:
    Servo servo;
  private:
    int _lPin;
    int _state;
    int _tiltMin;
    int _tiltMax;
    int _currentTilt;
    int _remaining_moves;
};

#endif