float windspeed = 0.0;
float generator = 5.0;
float RPM = 0;
float angle = 0;

void loop() {
  RPM = tacho->getRPM();

  if (Serial.available() >= 2) {
    int eStop = Serial.read();
    int pitch = Serial.read();
    angle = pitch;
    Serial.write((int)RPM);
    Serial.write((int)angle);
    
    digitalWrite(LED_BUILTIN, eStop);
  }

  int analogValue = analogRead(A0);
  // Rescale to potentiometer's voltage (from 0V to 5V):
  angle = floatMap(analogValue, 0, 90, 0, 5);
  pitchStepper->setAngle(angle);


}
