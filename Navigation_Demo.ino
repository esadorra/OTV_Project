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
float inches = 0.0;

//voltage reading pin
const int v = A5;
int value = 0;
float voltage = 0;
float R1 = 47000.0;
float R2 = 33000.0;

void setup() {
  delay(1000);
  Enes100.begin("Enginnering Elephants", HYDROGEN, 19, 1120, 12, 13);
  delay(1000);
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
  
  miniTest();

  /*
  float distance = activateUltrasonic();
  const float obstacleThresholdCm = 5.0;
  //navigation logic
  if(Enes100.getY() > 1){
    turnToAngle(-PI/2);
    moveForward();
    //mission
    if(d > 0 && d < obstacleThresholdCm && activateLimitSwitch()){
      motorOff();
      activatePhotoresistor();
      measureVoltage();
    }
    turnToAngle(PI/2);
  } else {
    turnToAngle(PI/2);
    //mission
    if(d > 0 && d < obstacleThresholdCm && activateLimitSwitch()){
      motorOff();
      activatePhotoresistor();
      measureVoltage();
    }
  }

  moveUntilY(1.85);
  turnToAngle(0.0);
  moveUntilX(3.0);
  turnToAngle(-PI/2);
  moveUntilY(1.48); 
  turnToAngle(0.0);
  moveUntilX(3.6);
  */
}

void loop() {
  activatePhotoresistor();   // prints dominant light
  activateLimitSwitch();    // prints switch state
  activateUltrasonic();     // prints distance
  measureVoltage();         // prints voltage
  delay(500);
}

void miniTest(){
  turnToAngle(PI/2 - 0.0692036732);
  motorOff();
  moveUntilY(1.85);
  motorOff();
  turnToAngle(0.0);
  motorOff();
  slideLeft();
  motorOff();
  moveUntilX(3.0);
  motorOff();
}

void measureVoltage(){
  value = analogRead(v);
  voltage = value * (5.0/1024)*((R1 + R2)/R2);
  Enes100.print("Voltage = ");
  Enes100.print(voltage);
}

//normalizing angle when we turn so that it do not turn the long way
float normalizeAngle(float angle){
    while (angle > PI) angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    return angle;
}

//measures distance using ultrasonic sensor
long activateUltrasonic(){
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor
  duration = pulseIn(echoPin, HIGH);

  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;  
   // Divide by 74 or multiply by 0.0135
  Enes100.print(inches);
  Enes100.print(" in, ");
  Enes100.print(cm);
  Enes100.print(" cm");


  delay(50);

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
  if (r2 > maxRead) { maxRead = p2; lightOn = "Red → No Power"; }
  if (r3 > maxRead) { maxRead = p3; lightOn = "Yellow → Low Power"; }
  if (r4 > maxRead) { maxRead = p4; lightOn = "Green → Full Power"; }
  if (r5 > maxRead) { maxRead = p5; lightOn = "Blue → Over Power"; }

  Enes100.print("Light on: ");
  Enes100.print(lightOn);
  delay(3000);
}

boolean activateLimitSwitch(){
  //Read the state of the limit switch
  int switchState = digitalRead(limitSwitchPin);

  // Check if the switch is pressed (state is LOW)
  if (switchState == LOW) {
    Enes100.println("Limit Switch Activated!");
    return true;
  } else {
    // The switch is not pressed
    Enes100.println("Limit Switch Not Active");
    return false;
  } 
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

        delay(40);
        motorOff();
        delay(20);
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

        delay(40);
        motorOff();
        delay(20);
    }

    motorOff();
}
