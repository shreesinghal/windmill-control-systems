class StepperMotor {
  
  double angle;
  const int dirPin = 2;
  const int stepPin = 3;
  const int stepsPerRevolution = 200;

  StepperMotor() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    this.angle = 0;
    
  }

  

  public void spinClockwise() {
    digitalWrite(dirPin, HIGH);
  }
  
  public void spinCounterclockwise() {
    digitalWrite(dirPin, LOW);
  }

  // resets the stepper to 0
  public void resetStepper() {
    
  }
  
}

//change in wind speed from one state to another surpasses a certain threshold


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
