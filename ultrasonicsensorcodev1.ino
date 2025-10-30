int trigPin = 3;    // Trigger
int echoPin = 6;    // Echo
long duration, cm, inches;
 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() { // The sensor is triggered by a HIGH pulse of 10 or more microseconds 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(250);

  // Above code measures distance using ultrasonic sensor
  // We know the distance between the mission start zone and the fuel cell. When the ultrasonic sensor transmits this distance, the OTV will stop rotating and go forwards
  // We will also include a check to make sure the angle by the aruco marker is also accurate
