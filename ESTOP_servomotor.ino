/*
ReNU 6/13/24 - Clara
First draft of code for E-stop using sparkfun redboard, servo motor, button, and switch
Will either use button or switch (idea is to use a pushbutton switch to keep the state)
We will change the servo motor out for the E-stop motor, and the redboard for the STM board
Will update to use millis timer to constantly check state of button
Will update to use OOP
*/

#include <Servo.h>

int switchPin = 11;
int estopButton = 10;

Servo myServo; 

//unsigned long cur_millis = 0;
//unsigned long prev_millis = 0;

int checkEstopButton();
int buttonPress = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  myServo.write(0); //initialize servo to be open, windmill is free
  delay(1000);

  pinMode(estopButton, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  myServo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonPress = checkEstopButton();

  if (buttonPress == 1){
    myServo.write(90);
    Serial.print("\nMoved the servo.");
    delay(1000);
  }
  else{
    myServo.write(0);
    Serial.print("\nBack to original spot");
    delay(1000);
  }
}

int checkEstopButton(){
  // check if the button is being pressed
  if (digitalRead(estopButton) == LOW){
    return 1;
  }
  else{
    return 0;
  }
}
