

class StepperMotor {
  private:
    double angle;
    const int dirPin;
    const int stepPin;
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
      //Serial.println("hi");
      if (newAngle >= 90 || newAngle < 0) {
        Serial.println("Degree value out of bounds");
      }

      else if (this->angle != newAngle) {

        // sets it counter 
        if (newAngle > this->angle) {
          this->spinCounterclockwise();
        }
        else {
          this->spinClockwise();
        }

        for(int x = 0; x < abs(this->angleToSteps(newAngle - this->angle)); x++)
        {
          digitalWrite(stepPin, HIGH);
          delayMicroseconds(3000);
          digitalWrite(stepPin, LOW);
          delayMicroseconds(3000);
        }
        this->angle = newAngle;
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

//change in wind speed from one state to another surpasses a certain threshold

StepperMotor *s;


void setup() {
  // put your setup code here, to run once:
  s = new StepperMotor();
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  float degree;
  Serial.println("Enter in a degree: ");
  if (Serial.available() > 0) {
    degree = Serial.parseFloat();
    Serial.println("calling");
    s->setAngle(degree);
  }

}
