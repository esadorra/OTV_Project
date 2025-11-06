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
}

void loop() {
  directionControl();
  delay(1000);
}

// This function changes motor directions to move the OTV through the range of turning/sliding movements
void directionControl() {

  // OTV FORWARD
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
  delay(2000);

  // OTV BACKWARD
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
  delay(2000);

  // OTV TURN RIGHT
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
  delay(2000);

  // OTV LEFT
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
  delay(2000);

  // OTV SLIDE LEFT
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
  delay(2000);

  // OTV SLIDE RIGHT
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
  delay(2000);

  // MOTORS OFF
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
  delay(2000);


}
