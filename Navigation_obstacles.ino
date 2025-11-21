#include "Enes100.h"
#include <math.h>
#include "Arduino.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

// ---------------- MOTOR PINS ----------------
const int in1L = 2;
const int in2L = 4;
const int in3L = 6;
const int in4L = 7;

const int in1R = 8;
const int in2R = 9;
const int in3R = 10;
const int in4R = 11;

// --------------- ULTRASONIC ------------------
const int trigPin = 3;
const int echoPin = 5;
long duration = 0;
float cm = 0.0;

// Target corridor X (middle between the two blocks)
const float TARGET_X = 1.20;

// =====================================================
// ================= MOTOR FUNCTIONS ====================
// =====================================================
void moveForward(){
  digitalWrite(in1L,HIGH);  digitalWrite(in2L,LOW);
  digitalWrite(in3L,HIGH);  digitalWrite(in4L,LOW);
  digitalWrite(in1R,HIGH);  digitalWrite(in2R,LOW);
  digitalWrite(in3R,HIGH);  digitalWrite(in4R,LOW);
}

void moveBackward(){
  digitalWrite(in1L,LOW);   digitalWrite(in2L,HIGH);
  digitalWrite(in3L,LOW);   digitalWrite(in4L,HIGH);
  digitalWrite(in1R,LOW);   digitalWrite(in2R,HIGH);
  digitalWrite(in3R,LOW);   digitalWrite(in4R,HIGH);
}

void slideLeft(){
  digitalWrite(in1L,LOW);   digitalWrite(in2L,HIGH);
  digitalWrite(in3L,HIGH);  digitalWrite(in4L,LOW);
  digitalWrite(in1R,HIGH);  digitalWrite(in2R,LOW);
  digitalWrite(in3R,LOW);   digitalWrite(in4R,HIGH);
}

void slideRight(){
  digitalWrite(in1L,HIGH);  digitalWrite(in2L,LOW);
  digitalWrite(in3L,LOW);   digitalWrite(in4L,HIGH);
  digitalWrite(in1R,LOW);   digitalWrite(in2R,HIGH);
  digitalWrite(in3R,HIGH);  digitalWrite(in4R,LOW);
}

void turnLeft(){
  digitalWrite(in1L,LOW);   digitalWrite(in2L,HIGH);
  digitalWrite(in3L,LOW);   digitalWrite(in4L,HIGH);
  digitalWrite(in1R,HIGH);  digitalWrite(in2R,LOW);
  digitalWrite(in3R,HIGH);  digitalWrite(in4R,LOW);
}

void turnRight(){
  digitalWrite(in1L,HIGH);  digitalWrite(in2L,LOW);
  digitalWrite(in3L,HIGH);  digitalWrite(in4L,LOW);
  digitalWrite(in1R,LOW);   digitalWrite(in2R,HIGH);
  digitalWrite(in3R,LOW);   digitalWrite(in4R,HIGH);
}

void motorOff(){
  digitalWrite(in1L,LOW); digitalWrite(in2L,LOW);
  digitalWrite(in3L,LOW); digitalWrite(in4L,LOW);
  digitalWrite(in1R,LOW); digitalWrite(in2R,LOW);
  digitalWrite(in3R,LOW); digitalWrite(in4R,LOW);
}

// =====================================================
// ==================== ULTRASONIC ======================
// =====================================================

float readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return -1;

  return (duration * 0.0343 / 2);
}

// =====================================================
// ================== ANGLE NORMALIZER =================
// =====================================================
float norm(float a){
  while(a > PI) a -= 2*PI;
  while(a < -PI) a += 2*PI;
  return a;
}

void turnTo(float ang){
  ang = norm(ang);

  while(true){
    float th = norm(Enes100.getTheta());
    float err = norm(ang - th);

    if(fabs(err) < 0.06) break;

    if(err > 0) turnLeft();
    else        turnRight();

    delay(20);
    motorOff();
    delay(10);
  }
  motorOff();
}

// =====================================================
// ======== MOVE FORWARD WITH ULTRASONIC AVOID =========
// =====================================================

void forwardWithAvoid() {
  float d = readUltrasonic();

  if (d > 0 && d < 25) {
    // Obstacle detected!
    motorOff();
    delay(50);

    // Try sliding left
    slideLeft();
    delay(300);
    motorOff();

    // Re-check
    d = readUltrasonic();
    if (d > 0 && d < 15) {
      // Try sliding right instead
      slideRight();
      delay(600);
      motorOff();
    }

    // If STILL blocked, back up
    d = readUltrasonic();
    if (d > 0 && d < 15) {
      moveBackward();
      delay(300);
      motorOff();
    }

    return;
  }

  // No obstacle → move forward
  moveForward();
}

// =====================================================
// ================== MAIN NAVIGATION ==================
// =====================================================

void setup() {
  Serial.begin(9600);
  Enes100.begin("Engineering Elephants", HYDROGEN, 19, 1120, 12, 13);

  pinMode(in1L, OUTPUT); pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT); pinMode(in4L, OUTPUT);
  pinMode(in1R, OUTPUT); pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT); pinMode(in4R, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  motorOff();
  delay(500);

  // Face positive X direction
  turnTo(0);
}

void loop() {

  float x = Enes100.getX();

  // Stop once past obstacles
  if (x > 2.8) {
    motorOff();
    Serial.println("Finished obstacle avoidance.");
    while(true);
  }

  forwardWithAvoid();

  delay(50);
  motorOff();
  delay(20);
}

