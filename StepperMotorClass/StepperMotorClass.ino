class StepperMotor {
  private:
    double angle;
    int dirPin;
    int stepPin;
    const int stepsPerRevolution = 200;
    const double anglePerStep = 1.8;
    bool run;

  public: 
  StepperMotor(int dirPinIn, int stepPinIn) {
    dirPin = dirPinIn;
    stepPin = stepPinIn;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    this->angle = 0;
  }

    // negative 
    void spinClockwise() {
      digitalWrite(dirPin, HIGH);
    }
    
    // positive
    void spinCounterclockwise() {
      digitalWrite(dirPin, LOW);
    }

    // resets the stepper to 0
    void resetStepper() {
      // send it the negative of angle
      this->setAngle(0);
    }

    // Set to angle
    // Sets to the best approximation for angle
    // Divides the angle by 1.8
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
      Serial.println(angleDiff);
    
      int steps = abs(angleToSteps(angleDiff)); // Calculate the number of steps needed
    
      // Move the stepper motor to the desired position
      for (int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(3000);  // Adjust timing as needed for stepper speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(3000);
      }
    
      // After moving, update the angle to the new value
      this->angle = newAngle;
    }

    void spinForever(){
      spinClockwise();
      while(true) {
        digitalWrite(stepPin, HIGH);

      }
    }

    // changes the angle by the amount given
    void changeAngle(double inc) {
      this->setAngle(this->angle + inc);
    }

  private:
    int angleToSteps(double angle) {
      return angle / anglePerStep;
    }

  
};

