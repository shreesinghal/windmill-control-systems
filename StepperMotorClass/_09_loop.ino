float windspeed = 0.0;
float generator = 5.0;
float RPM = 0;
float angle = 0;

void loop() {
  RPM = tacho->getRPM();
  Serial.println(RPM);


  int analogValue = analogRead(A0);
  // Rescale to potentiometer's voltage (from 0V to 5V):
  angle = floatMap(analogValue, 0, 90, 0, 5);
  pitchStepper->setAngle(angle);

  // print out the value you read:
  Serial.print(", Angle: ");
  Serial.println(angle);
  delay(1000);

}