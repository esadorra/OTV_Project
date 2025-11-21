#include "Enes100.h"
#include <math.h>
#include "Arduino.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

// ---------------- MOTOR PINS ----------------

// Left side motors (A + B)
const int in1L = 2;
const int in2L = 4;
const int in3L = 6;
const int in4L = 7;

// Right side motors (C + D)
const int in1R = 8;
const int in2R = 9;
const int in3R = 10;
const int in4R = 11;

// --------------- LIMIT SWITCH ---------------
const int limitSwitchPin = 1;

// --------------- PHOTORESISTORS --------------
const int p1 = A0;
const int p2 = A1;
const int p3 = A2;
const int p4 = A3;
const int p5 = A4;

// --------------- ULTRASONIC ------------------
const int trigPin = 3;
const int echoPin = 13;   // FIXED & RELIABLE INPUT PIN

long duration = 0;
float cm = 0.0;

// --------------- VOLTAGE PIN -----------------
const int v = A5;


// =======================================================
// ======================== SETUP ========================
// =======================================================

void setup() {

  Serial.begin(9600);
  
  // Motor pins
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT);
  pinMode(in4L, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);

  // Inputs
  pinMode(limitSwitchPin, INPUT_PULLUP);
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  pinMode(p3, INPUT);
  pinMode(p4, INPUT);
  pinMode(p5, INPUT);
  pinMode(v, INPUT);

  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Motors off at start
  motorOff();

  // Test stopping using ultrasonic
  miniTest3();
}

void loop() {
  // Just sensor prints
  activatePhotoresistor();
  activateLimitSwitch();
  activateUltrasonic();
  measureVoltage();
  delay(500);
}



// =======================================================
// ==================== TEST FUNCTIONS ===================
// =======================================================

void miniTest3() {
  Serial.println("Running ultrasonic stop test...");
  moveUntilDistance(10.0);   // stop at 10 cm
  motorOff();
}



// =======================================================
// ====================== ULTRASONIC =====================
// =======================================================

float activateUltrasonic() {

  // Trigger 10µs pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo with timeout (20ms)
  duration = pulseIn(echoPin, HIGH, 20000);

  // No echo → invalid
  if (duration == 0) {
    Serial.println("Ultrasonic: No echo");
    return -1;
  }

  // Convert to distance
  cm = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  return cm;
}


// ---------------- Move until target distance ----------------

void moveUntilDistance(float stopCm) {

  moveForward();

  while (true) {
    float distance = activateUltrasonic();

    // Valid reading & close enough
    if (distance > 0 && distance <= stopCm) {
      Serial.println("STOPPING — Object detected.");
      motorOff();
      break;
    }

    delay(30);
  }
}



// =======================================================
// =================== OTHER SENSORS =====================
// =======================================================

void measureVoltage() {
  int value = analogRead(v);
  float voltage = value * (5.0 / 1023.0);

  Serial.print("Voltage: ");
  Serial.println(voltage);
}

boolean activateLimitSwitch() {
  int state = digitalRead(limitSwitchPin);

  if (state == LOW) {
    Serial.println("Limit Switch: PRESSED");
    return true;
  } else {
    Serial.println("Limit Switch: Not pressed");
    return false;
  }
}

void activatePhotoresistor() {
  int r1 = analogRead(p1);
  int r2 = analogRead(p2);
  int r3 = analogRead(p3);
  int r4 = analogRead(p4);
  int r5 = analogRead(p5);

  int minVal = r1;
  String light = "White (Not Working)";

  if (r2 < minVal) { minVal = r2; light = "Red (No Power)"; }
  if (r3 < minVal) { minVal = r3; light = "Yellow (Low Power)"; }
  if (r4 < minVal) { minVal = r4; light = "Green (Full Power)"; }
  if (r5 < minVal) { minVal = r5; light = "Blue (Over Power)"; }

  Serial.print("Light on: ");
  Serial.println(light);
}



// =======================================================
// ====================== MOVEMENT =======================
// =======================================================

void moveForward() {
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}

void moveBackward() {
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}

void turnLeft() {
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}

void turnRight() {
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}

void slideLeft() {
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}

void slideRight() {
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}

void motorOff() {
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, LOW);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, LOW);
}



// =======================================================
// =============== ENES100 POSITIONING ===================
// =======================================================

float normalizeAngle(float angle){
  while(angle > PI) angle -= 2*PI;
  while(angle < -PI) angle += 2*PI;
  return angle;
}

void turnToAngle(float targetAngle){
  const float threshold = 0.03;

  targetAngle = normalizeAngle(targetAngle);

  while(true){
    float current = normalizeAngle(Enes100.getTheta());
    float error = normalizeAngle(targetAngle - current);

    if(abs(error) <= threshold)
      break;

    if(error > 0)
      turnLeft();
    else
      turnRight();

    delay(40);
    motorOff();
    delay(20);
  }

  motorOff();
}

void moveUntilY(float targetY){
  while(true){
    float y = Enes100.getY();
    float error = targetY - y;

    if(abs(error) < 0.05)
      break;

    if(error > 0) moveForward();
    else moveBackward();

    delay(40);
    motorOff();
    delay(20);
  }

  motorOff();
}

void moveUntilX(float targetX){
  while(true){
    float x = Enes100.getX();
    float error = targetX - x;

    if(abs(error) < 0.05)
      break;

    if(error > 0) moveForward();
    else moveBackward();

    delay(40);
    motorOff();
    delay(20);
  }

  motorOff();
}
