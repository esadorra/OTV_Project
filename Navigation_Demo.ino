#include "Enes100.h"
#include <math.h>
#include "Arduino.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

// H-bridge for left side wheels
// Motor A direction control pins
const int in1L = 2;
const int in2L = 4;
// Motor B direction control pins
const int in3L = 6;
const int in4L = 7;
// H-bridge for right side wheels
// Motor C direction control pins
const int in1R = 8;
const int in2R = 9;
// Motor D direction control pins
const int in3R = 10;
const int in4R = 11;

//limit_switch pin
const int limitSwitchPin = 1;

//photoresistor pin & variables related to activating the photoresistor
const int p1 = A0;
const int p2 = A1;
const int p3 = A2;
const int p4 = A3;
const int p5 = A4;
int maxRead = 0;
String lightOn = "";

//ultrasonic sensor pins
const int trigPin = 3;
const int echoPin = 5;
long duration = 0;
float cm = 0.0;

//voltage reading pin
const int v = A5;
int value = 0;

void setup() {
  Enes100.begin("Enginnering Elephants", HYDROGEN, 19, 1116, 12, 13);
  Enes100.println("Starting...");
  //Serial.begin(9600);

  // Set all the motor control pins to outputs
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT);
  pinMode(in4L, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);

  pinMode(limitSwitchPin, INPUT_PULLUP);

  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  pinMode(p3, INPUT);
  pinMode(p4, INPUT);
  pinMode(p5, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(v, INPUT);

  // Turn off motors - Initial state
  motorOff();
  
  //whole navigation logic
  const float obstacleThresholdCm = 10.0;
  if(Enes100.getY() > 1){
    turnToAngle((-PI/2)-0.08);
    moveUntilY(0.4);
    turnToAngle(-PI/2);
    slideLeftUntilX(0.3);
    moveUntilDistance(obstacleThresholdCm);
    delay(1000);
    //mission
    if(activateLimitSwitch()){
      activatePhotoresistor();
      measureVoltage();
    }
    moveBackward();
    delay(2000);
    turnToAngle(PI/2);
    slideRight();
    delay(2500);
    motorOff();
  } else {
    turnToAngle((PI/2)-0.08);
    moveUntilY(1.1);
    turnToAngle((PI/2)-0.08);
    slideRightUntilX(0.3);
    /*
    while(activateLimitSwitch()){
      slideLeft();
    }
    */
    moveUntilDistance(obstacleThresholdCm);
    delay(1000);
    /*
    while(!isLimitSwitchActive()){
      Enes100.println("Sliding Left");
      slideLeft();
      delay(10);
    }
    */
    motorOff();
    delay(500);
    moveForward();
    delay(500);
    //mission
    activatePhotoresistor();
    measureVoltage();
    motorOff();
    delay(2000);
    moveBackward();
    delay(2500);
    slideRight();
    delay(3000);
    motorOff();
  }

  moveUntilY(1.85);
  turnToAngle(0.0);
  slideLeft();
  delay(2000);
  moveUntilX(3.0);
  slideRight();
  delay(3000);
  moveUntilX(3.6);
}

void loop() {
  activatePhotoresistor();   // prints dominant light
  activateLimitSwitch();    // prints switch state
  activateUltrasonic();     // prints distance
  measureVoltage();         // prints voltage
  delay(500);
}

void miniTest(){
  turnToAngle(PI/2 - 0.08);
  moveUntilY(1.85);
  turnToAngle(0.0);
  slideLeft();
  delay(2000);
  moveUntilX(3.0);
  motorOff();
  slideRight();
  moveUntilX(3.6);
}

void miniTest3(){
  moveUntilDistance(10.0);
  motorOff();
}

//moves the OTV until the distance reaches below stopCm
void moveUntilDistance(float stopCm){
  moveForward();

  while(true){
    float distance = activateUltrasonic();

    if(distance > 0 && distance <= stopCm){
      motorOff();
      break;
    }
    
    delay(30);
  }
}

void measureVoltage(){
  value = analogRead(v);
  float voltage = value * (5.0/1023.0);
  Enes100.println("Voltage = ");
  Enes100.println(voltage);
}

//normalizing angle when we turn so that it do not turn the long way
float normalizeAngle(float angle){
    while (angle > PI) angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    return angle;
}

//measures distance using ultrasonic sensor
float activateUltrasonic(){
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor
  duration = pulseIn(echoPin, HIGH, 20000);

  if(duration == 0){
    return -1;
  }

  cm = duration * 0.0343 / 2;     // Divide by 29.1 or multiply by 0.0343 
  Enes100.print("Distance: ");
  Enes100.print(cm);
  Enes100.print(" cm");

  return cm;
}

void activatePhotoresistor(){
  //reads photoresistor values
  int r1 = analogRead(p1); //White → Not Working   
  int r2 = analogRead(p2); //Red → No Power
  int r3 = analogRead(p3); //Yellow → Low Power
  int r4 = analogRead(p4); //Green → Full Power
  int r5 = analogRead(p5); //Blue → Over Power

  maxRead = r1;
  lightOn = "White → Not Working";
  if (r2 < maxRead) { maxRead = r2; lightOn = "Red → No Power"; }
  if (r3 < maxRead) { maxRead = r3; lightOn = "Yellow → Low Power"; }
  if (r4 < maxRead) { maxRead = r4; lightOn = "Green → Full Power"; }
  if (r5 < maxRead) { maxRead = r5; lightOn = "Blue → Over Power"; }

  Enes100.print("A0, white: ");
  Enes100.println(p1);
  Enes100.print("A1, red: ");
  Enes100.println(p2);
  Enes100.print("A2, yellow: ");
  Enes100.println(p3);
  Enes100.print("A3, green: ");
  Enes100.println(p4);
  Enes100.print("A4, blue: ");
  Enes100.println(p5);
  
  Enes100.print("Light on: ");
  Enes100.println(lightOn);
  delay(1000);
}

bool isLimitSwitchActive() {
    return digitalRead(limitSwitchPin) == LOW;
}

//Going Forward
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

//Moving backward
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

//turning right
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

//turning left
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

//turning the motor off
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

//turning the OTV to an assigned angle
void turnToAngle(float targetAngle){
    const float threshold = 0.03; //need adjustment

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

void slideRightUntilX(float targetX){
  while(true){
        float currentX = Enes100.getX();
        float errorX = targetX - currentX;

        if(abs(errorX) < 0.05){
          break;
        }

        if(errorX > 0){
          slideRight();
        } else {
          slideLeft();
        }
        
        /*
        delay(40);
        motorOff();
        delay(20);
        */
    }

    motorOff();
}

void slideLeftUntilX(float targetX){
  while(true){
        float currentX = Enes100.getX();
        float errorX = targetX - currentX;

        if(abs(errorX) < 0.05){
          break;
        }

        if(errorX > 0){
          slideLeft();
        } else {
          slideRight();
        }

        /*
        delay(40);
        motorOff();
        delay(20);
        */
    }

    motorOff();
}

//moving forward until an assigned Y value
void moveUntilY(float targetY){
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

        /*
        delay(40);
        motorOff();
        delay(20);
        */
    }

    motorOff();
}

//moving forward until an assigned X value
void moveUntilX(float targetX){
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

        /*
        delay(40);
        motorOff();
        delay(20);
        */
    }

    motorOff();
  }
