void loop() {
  // manual stepper motor angle change
  float degree;
  Serial.println("Enter in a degree: ");
  degree = Serial.parseFloat();
  s->changeAngle(degree);
  Serial.println("done");

  
  Serial.println(RPM);
}