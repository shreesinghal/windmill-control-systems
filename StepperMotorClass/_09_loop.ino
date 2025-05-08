/*

*/


float windspeed = 0.0;
float generator = 5.0;
float angle = 0;





void loop() {
  // manual stepper motor angle change
  // float degree;
  // Serial.println("Enter in a degree: ");
  // degree = Serial.parseFloat();
  // s->changeAngle(degree);
  // Serial.println("done");
  
  // t->measureSensor();

  // digitalWrite(PowerPin, HIGH);
  // digitalWrite(GndPin, LOW);

  // return;
  
  // l -> FullyRetract();
  
  s->spinForever();
  angle += 10;
  delay(1000);
  
  l -> FullyExtend();
  
  delay(100);
  return;

  
  // Serial.println(t->getRPM());

  // update readings
  // windspeed

 
  
    // send data
  if (digitalRead(MH_SENSOR) == HIGH) {
    Serial.write(0b11111111);
  }
  else {
    Serial.write(0b0);
  }
  
  t->measureSensor();
  Serial.write(t->getRPM());
    Serial.write(0b0);
  delay(10);
  if (Serial.available() > 0) {
    
    // read the angle
    int message = Serial.read();
    int pitch = message & 0b11111110;
    int e_stop = message & 0b1;
    
    // if (digitalRead(MH_SENSOR) == HIGH) {
    analogWrite(5, message);
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
}