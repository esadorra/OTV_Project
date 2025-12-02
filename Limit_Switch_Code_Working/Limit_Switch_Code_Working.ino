#include <Enes100.h>

const int limitSwitchPin = 0; // The pin the limit switch is connected to

void setup() {
  // Initialize serial communication for debugging
  Enes100.begin("Engineering Elephants", HYDROGEN, 19, 1116, 12, 13);
  Enes100.println("Starting...");
  // Set the pin as an input with an internal pull-up resistor.
  // This means the pin will read HIGH by default and LOW when the switch is pressed.
  pinMode(limitSwitchPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the limit switch
  int switchState = digitalRead(limitSwitchPin);

  // Check if the switch is pressed (state is LOW)
  if (switchState == LOW) {
    Enes100.println("Limit Switch Activated!");
  } else {
    // The switch is not pressed
   Enes100.println("Limit Switch Not Active");
  }

  // Add a small delay to prevent the output from flooding the serial monitor
  delay(100);
}
