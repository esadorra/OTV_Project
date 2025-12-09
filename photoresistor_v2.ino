int p1, p2, p3, p4, p5;
int maxRead;
String lightOn;
 //whichever photoresistor transmits the highest light intensity is indicated to be on

void setup() {
  Serial.begin(9600);
}

void loop() {
  //reads photoresistor values
  // might have to change pin number for each sensor
 p1 = analogRead(A2); //White → Not Working   
 p2 = analogRead(A1); //Red → No Power
 p3 = analogRead(A0); //Yellow → Low Power
 p4 = analogRead(A3); //Green → Full Power
 p5 = analogRead(A4); //Blue → Over Power

  //Determine which sensor has the highest reading
  maxRead = p1;
  lightOn = "White → Not Working";
  if (p2 < maxRead) { maxRead = p2; lightOn = "Red → No Power"; }
  if (p3 < maxRead) { maxRead = p3; lightOn = "Yellow → Low Power"; }
  //if (p4 < maxRead) { maxRead = p4; lightOn = "Green → Full Power"; }
  if (p5 < maxRead) { maxRead = p5; lightOn = "Blue → Over Power"; }

  if(p1 > 900 && p2 > 900 && p3 > 900 && p5 > 900){
    maxRead = p4;
    lightOn = "Green --> Full Power";
  }

  Serial.print("A2, white: ");
  Serial.println(p1);
  Serial.print("A1, red: ");
  Serial.println(p2);
  Serial.print("A0, yellow: ");
  Serial.println(p3);
  Serial.print("A3, green: ");
  Serial.println(p4);
  Serial.print("A4, blue: ");
  Serial.println(p5);
  
  Serial.print("Light on: ");
  Serial.println(lightOn);
 delay(1000);

}
