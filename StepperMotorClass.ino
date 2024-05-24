class StepperMotor {
  
  double angle;
  const int dirPin = 2;
  const int stepPin = 3;
  const int stepsPerRevolution = 200;
  const double anglePerStep = 1.8;

  StepperMotor() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    this.angle = 0;
    
  }

  

  // negative 
  public void spinClockwise() {
    digitalWrite(dirPin, HIGH);
  }
  
  // positive
  public void spinCounterclockwise() {
    digitalWrite(dirPin, LOW);
  }

  // resets the stepper to 0
  public void resetStepper() {
    // send it the negative of angle

  }

  // Set to angle
  // Sets to the best approximation for angle
  // Divides the angle by 1.8
  public void setAngle(double newAngle) {
    if (this.angle != newAngle) {
      // sets it counter 
      if (newAngle > this.angle) {
        this.spinCounterclockwise();
      }
      else {
        this.spinClockwise();
      }

      for(int x = 0; x < this.angleToSteps(newAngle); x++)
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(3000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(3000);
      }
      this.angle = newAngle;

    }
  }

  private int angleToSteps(double angle) {
    return angle / anglePerStep;
  }

  
}

//change in wind speed from one state to another surpasses a certain threshold


void setup() {
  // put your setup code here, to run once:
  StepperMotor s = new StepperMotor();
  float degree;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  Serial.println("Enter in a degree: ");
  if (Serial.available() > 0) {
    degree = Serial.parseFloat();
    s.setAngle(degree);
  }

}
