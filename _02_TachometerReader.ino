// class StepperMotor {
//   private:
//     double RPM;

//   public:
//   StepperMotor(int dirPinIn, int stepPinIn) {
//     dirPin = dirPinIn;
//     stepPin = stepPinIn;
//     pinMode(stepPin, OUTPUT);
//     pinMode(dirPin, OUTPUT);
//     this->angle = 0;
//   }

//   void measureSensor() {

//     if (digitalRead(MH_SENSOR) == HIGH && prevState == false) {       //End of last blade
//       startTime = millis();
//       Serial.println("END OF BLADE");
//       prevState = true;
//     }

//     if (digitalRead(MH_SENSOR) == LOW && prevState == true) {         //Start of next blade
//       period = millis() - startTime;
//       RPM = 60000 / period;
      
//       Serial.println("START OF BLADE");
//       prevState = false;
//     }
//   }


// }