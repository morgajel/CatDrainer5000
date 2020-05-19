#include <Arduino.h>
#include <unity.h>
#include "laser.h"
#include <Servo.h>

Servo servo;
Laser laser(13, servo);

void test_servo(void) {


}

// void tearDown(void) {
//     // clean stuff up here
//     laser = 0;
// }

// void test_laserOn(void) {

//     TEST_ASSERT_NULL(laser.laserOn());
// }

 void setup(){
     delay(2000); // service delay
     UNITY_BEGIN();

     RUN_TEST(test_servo);

     UNITY_END(); // stop unit testing
}

void loop(){
}
