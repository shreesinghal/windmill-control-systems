/*
ReNU 6/20/24 - Clara
First draft of code for E-stop using sparkfun redboard, servo motor, button, and switch
Will either use button or switch (idea is to use a pushbutton switch to keep the state)
We will change the servo motor out for the E-stop motor, and the redboard for the STM board
*/

#include <StepperMotorClass.ino>
#include <String>

class Estop {
  public: 
    int estopComponent;
    int servoPin;
    int buttonPress = 0;
    String runMethod; //change this to "button" to use button
    StepperMotor motor;

    Estop(){
      this->estopComponent = 10;
      this->motor = new StepperMotor(2, 3);
      this->runMethod = "switch";
      pinMode(this->estopComponent, INPUT_PULLUP);
    }
    Estop(int estop, int dirPin, int stepPin, String run){
      this->estopComponent = estop;
      this->motor = new StepperMotor(dirPin, stepPin);
      this->runMethod = run;
    }

    void checkEStopComponent() {
      int state;
      if (digitalRead(this->estopComponent) == LOW){
        this->buttonPress = 1;
        this->setAngleStepper(90);
      }
      else{
        this->buttonPress = 0;
        this->setAngleStepper(0);
      }
    }

void setAngleStepper(int angle) {
  this->motor->setAngle(angle);
}
};

Estop reNU_windmill(10, 9, 2, 3); //create an object of Estop class

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  reNU_windmill->setAngleStepper(0); //initialize servo to be open, windmill is free
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  reNU_windmill->checkEStopComponent();
}
