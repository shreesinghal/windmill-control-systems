float windspeed = 0.0;
float generator = 5.0;
float RPM = 0;
float angle = 0;

void loop() {

  tacho->measureSensor();
  RPM = tacho->getRPM();
  // Serial.println(RPM);
  
  if (RPM > 60) {
    pitchStepper->changeAngle(-5);
  }

  if (RPM < 60) {
    pitchStepper->changeAngle(+5);
  }

  // pitchStepper->changeAngle(90);
  // delay(100);
  
  // l -> FullyExtend();
  
  // delay(100);
  // return;

  
  // if (digitalRead(MH_SENSOR) == HIGH) {
  //   Serial.write(0b11111111);
  // }
  // else {
  //   Serial.write(0b0);
  // }
  
  // t->measureSensor();
  // Serial.write(t->getRPM());
  //   Serial.write(0b0);
  // delay(10);
  // if (Serial.available() > 0) {
    
    // read the angle
    // int message = Serial.read();
    // int pitch = message & 0b11111110;
    // int e_stop = message & 0b1;
    
    // if (digitalRead(MH_SENSOR) == HIGH) {
    // analogWrite(5, message);
    // }
    // else {
    //   digitalWrite(4, LOW);
    // }
    
    
    // set_angle(angle);
  
    
      // Serial.write(1);
      // Serial.write(1);
    
    // Serial.write(5);
    // Serial.write(7);
    // Serial.print(message);
    // Serial.print('n');
    // Serial.println(generator)
    // Serial.println()
  
    

}