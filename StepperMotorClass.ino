#include <LiquidCrystal.h>

const int LED = 13, MH_SENSOR = 7, RESET_BTN = 6;
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2; //lcd
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

bool prevState = false;
float RPM = 0, period = 0;
unsigned long startTime = 0;

class StepperMotor {

  private:
    double angle;
    const int dirPin = 8;
    const int stepPin = 9;
    const int stepsPerRevolution = 200;
    const double anglePerStep = 1.8;
    bool run;

  public: 
  StepperMotor() {
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
      // send it the negative of angleen
      this->setAngle(0);
    }

    // Set to angle
    // Sets to the best approximation for angle
    // Divides the angle by 1.8
    void setAngle(double newAngle) {
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

  // Tachometer SetUp
  // MH Sensor setup:
  pinMode(LED, OUTPUT);
  pinMode(MH_SENSOR, INPUT);
  pinMode(RESET_BTN, INPUT);

  // LCD Display Setup
  lcd.begin(16, 2);
  lcd.print("RPS:");
  lcd.setCursor(0, 1);

  // Serial Console Setup:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  float degree;
  if (Serial.available() > 0) {
    degree = Serial.parseFloat();
    Serial.println("calling");
    s->setAngle(degree);
  }

  // Tachometer Loop
  // put your main code here, to run repeatedly:
  if (digitalRead(MH_SENSOR) == HIGH && prevState == false) {       //End of last blade
    digitalWrite(LED, HIGH);
    startTime = millis();    

    Serial.println("END OF BLADE");

    prevState = true;
  }

  if (digitalRead(MH_SENSOR) == LOW && prevState == true) {         //Start of next blade
    digitalWrite(LED, LOW);
    period = millis() - startTime;
    RPM = 60000 / period;
    
    Serial.println(RPM);
    Serial.println("START OF BLADE");

    prevState = false;
  }


  lcd.clear();
  lcd.print("RPM");
  lcd.setCursor(0, 1);
  lcd.print(RPM); // round to nearest .00

}
