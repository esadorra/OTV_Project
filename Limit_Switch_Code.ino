const int limitSwitchPin = 2; // The pin the limit switch is connected to

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set the pin as an input with an internal pull-up resistor.
  // This means the pin will read HIGH by default and LOW when the switch is pressed.
  pinMode(limitSwitchPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the limit switch
  int switchState = digitalRead(limitSwitchPin);

  // Check if the switch is pressed (state is LOW)
  if (switchState == LOW) {
    Serial.println("Limit Switch Activated!");
  } else {
    // The switch is not pressed
    Serial.println("Limit Switch Not Active");
  }

  // Add a small delay to prevent the output from flooding the serial monitor
  delay(100);
}
