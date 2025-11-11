#include "Enes100.h"
#include <math.h>
#include "Arduino.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

// H-bridge for left side wheels
// Motor A direction control pins
int in1L = 2;
int in2L = 4;
// Motor B direction control pins
int in3L = 6;
int in4L = 7;
// H-bridge for right side wheels
// Motor C direction control pins
int in1R = 8;
int in2R = 9;
// Motor D direction control pins
int in3R = 10;
int in4R = 11;

void setup() {
  Enes100.begin("Enginnering Elephants", HYDROGEN, 205, 1116, 12, 13);
  Enes100.println("Starting...");

  // Set all the motor control pins to outputs
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT);
  pinMode(in4L, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, LOW);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, LOW);

  turnToAngle(PI/2);
  moveForwardUntilY(1.85);
  turnToAngle(0.0);
  moveForwardUntilX(3.0);
  turnToAngle(-PI/2);
  moveForwardUntilY(1.48);
  turnToAngle(0.0);
  moveForwardUntilX(3.6);
}

void loop() {
  delay(150);
}

float normalizeAngle(float angle){
    while (angle > PI) angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    return angle;
}

void moveForward(){
  //A
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  //B
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  //C
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  //D
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}

void moveBackward(){
  //A
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  //B
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  //C
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  //D
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}

void turnRight(){
  //A
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  //B
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  //C
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  //D
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}

void turnLeft(){
  //A
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  //B
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  //C
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  //D
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}

/*
void slideLeft(){
  //A
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  //B
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  //C
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  //D
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}

void slideRight(){
  //A
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  //B
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  //C
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  //D
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}
*/

void motorOff(){
  //A
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  //B
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, LOW);
  //C
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);
  //D
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, LOW);
}

void turnToAngle(float targetAngle){
    const float threshold = 0.05;

    targetAngle = normalizeAngle(targetAngle);
    
    while (true) {
        float currentAngle = normalizeAngle(Enes100.getTheta());
        float error = normalizeAngle(targetAngle - currentAngle);
        
        if (abs(error) <= threshold) {
            break;
        }
        
        if(error > 0){
          turnLeft();
        } else {
          turnRight();
        }

        delay(40); 
        motorOff();
        delay(20);
    }
    
    motorOff();
}

void moveForwardUntilY(float targetY){
    while(true){
        float currentY = Enes100.getY();
        float errorY = targetY - currentY;

        if(abs(errorY) < 0.05){
            break;
        }

        if(errorY > 0){
          moveForward();
        } else{
          moveBackward();
        }

        delay(40);
        motorOff();
        delay(20);
    }

    motorOff();
}

void moveForwardUntilX(float targetX){
    while(true){
        float currentX = Enes100.getX();
        float errorX = targetX - currentX;

        if(abs(errorX) < 0.05){
            break;
        }

        if(errorX > 0){
          moveForward();
        } else {
          moveBackward();
        }

        delay(40);
        motorOff();
        delay(20);
    }

    motorOff();
}