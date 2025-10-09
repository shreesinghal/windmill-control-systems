class StepperMotor {
  private:
    double angle = 0;
    int dirPin;
    int stepPin;
    const int stepsPerRevolution = 200;
    const double anglePerStep = 360 / stepsPerRevolution;
    bool run;

    int angleToSteps(double angle) {
      return angle / anglePerStep;
    }

  public: 
    StepperMotor(int dirPinIn, int stepPinIn) {
      dirPin = dirPinIn;
      stepPin = stepPinIn;

      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
    }

    // negative changeAngle
    void spinClockwise() {
      digitalWrite(dirPin, HIGH);
    }
    
    // positive changeAngle
    void spinCounterclockwise() {
      digitalWrite(dirPin, LOW);
    }

    // resets the stepper to 0 deg
    void resetStepper() {
      // send it the negative of angle
      this->setAngle(0);
    }

    // set to the given angle
    void setAngle(double newAngle) {    
      // Calculate the difference between the target angle and the current angle
      double angleDiff = newAngle - this->angle;

      if (angleDiff < 0) {
        // Move clockwise if the target angle is smaller
        this->spinClockwise();
      } else {
        // Move counterclockwise if the target angle is larger
        this->spinCounterclockwise();
      }
    
      int steps = abs(angleToSteps(angleDiff)); // Calculate the number of steps needed
    
      // Move the stepper motor to the desired position
      for (int i = 0; i < steps; i++) {
        Serial.println("i");
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(3000);  // Adjust timing as needed for stepper speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(3000);
      }
    
      // after moving, update the angle to the new value
      this->angle = newAngle;
    }

    void spin100StepsToTest(){
      spinClockwise();
      for (int i = 0; i < 100; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(3000);  // Adjust timing as needed for stepper speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(3000);
      }
    }

    // changes the angle by the amount given
    void changeAngle(double inc) {
      this->setAngle(this->angle + inc);
    }
};

