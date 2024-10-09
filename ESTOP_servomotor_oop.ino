/*
ReNU 6/20/24 - Clara
First draft of code for E-stop using sparkfun redboard, servo motor, button, and switch
Will either use button or switch (idea is to use a pushbutton switch to keep the state)
We will change the servo motor out for the E-stop motor, and the redboard for the STM board
*/

#include <Servo.h>

class Estop {
  public: 
    int switchPin;
    int estopButton;
    int servoPin;
    int buttonPress = 0;
    String runMethod; //change this to "button" to use button
    StepperMotor motor;

    Estop(){
      this->switchPin = 11;
      this->estopButton = 10;
      this->motor = new StepperMotor(2, 3);
      this->runMethod = "switch";
      pinMode(this->estopButton, INPUT_PULLUP);
      pinMode(this->switchPin, INPUT_PULLUP);
    }
    Estop(int switch1, int button1, int dirPin, int stepPin, String run){
      this->switchPin = switch1;
      this->estopButton = button1;
      this->motor = new StepperMotor(dirPin, stepPin);
      this->runMethod = run;
    }
    void checkEstopButton(){
      // check if the button is being pressed
      int state;
      if (digitalRead(estopButton) == LOW){
        state = 1;
      }
      else{
        state = 0;
      }
      buttonPress = state;
    }
    void checkEstopSwitch(){
      int state;
      if (digitalRead(switchPin) == LOW){
        state = 1;
      }
      else{
        state = 0;
      }
      buttonPress = state;
    }

void setAngleStepper(int angle) {
  motor->setAngle(degree);
}
};

Estop reNU_windmill(11,10,9, 2, 3); //create an object of Estop class

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  reNU_windmill->setAngleStepper(0); //initialize servo to be open, windmill is free
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (reNU_windmill.runMethod == "button"){
    reNU_windmill.checkEstopButton();
  }
  else if (reNU_windmill.runMethod == "switch") {
    reNU_windmill.checkEstopSwitch();
  }

  if (reNU_windmill.buttonPress == 1){
    reNU_windmill.myServo.write(90);
    Serial.print("\nMoved the servo.");
    delay(1000);
  }
  else{
    reNU_windmill.myServo.write(0);
    Serial.print("\nBack to original spot");
    delay(1000);
  }
}
