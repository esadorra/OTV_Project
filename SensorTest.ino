
#include "Arduino.h"

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

  pinMode(limitSwitchPin, INPUT_PULLUP);

  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  pinMode(p3, INPUT);
  pinMode(p4, INPUT);
  pinMode(p5, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(v, INPUT);


}

void loop() {
  activatePhotoresistor();   // prints dominant light
  activateLimitSwitch();    // prints switch state
  activateUltrasonic();     // prints distance
  measureVoltage();         // prints voltage
  delay(500);
}

void measureVoltage(){
  value = analogRead(v);
  voltage = value * (5.0/1024)*((R1 + R2)/R2);
  Serial.print("Voltage = ");
  Serial.print(voltage);
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
  Serial.print(inches);
  Serial.print(" in, ");
  Serial.print(cm);
  Serial.print(" cm");


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
  if (r2 < maxRead) { maxRead = r2; lightOn = "Red → No Power"; }
  if (r3 < maxRead) { maxRead = r3; lightOn = "Yellow → Low Power"; }
  if (r4 < maxRead) { maxRead = r4; lightOn = "Green → Full Power"; }
  if (r5 < maxRead) { maxRead = r5; lightOn = "Blue → Over Power"; }

  Serial.print("Light on: ");
  Serial.print(lightOn);
  delay(3000);
}

boolean activateLimitSwitch(){
  //Read the state of the limit switch
  int switchState = digitalRead(limitSwitchPin);

  // Check if the switch is pressed (state is LOW)
  if (switchState == LOW) {
    Serial.println("Limit Switch Activated!");
    return true;
  } else {
    // The switch is not pressed
    Serial.println("Limit Switch Not Active");
    return false;
  } 
}
