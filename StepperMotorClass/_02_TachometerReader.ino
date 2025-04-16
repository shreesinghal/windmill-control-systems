class Tachometer {
  private:
    double RPM;
    int pin;
    bool prevState;
    long startTime;
    long period;
     
    // float[] rpms = float[50]
    // float[] stack = float[50]

  public:
  Tachometer(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
    this->RPM = 0.0;
    this->prevState = false;
  }


  void measureSensor() {

    if (digitalRead(pin) == HIGH && prevState == false) {       //End of last blade
      startTime = millis();
      Serial.println("END OF BLADE");
      prevState = true;
    }

    if (digitalRead(pin) == LOW && prevState == true) {         //Start of next blade
      period = millis() - startTime;
      RPM = 60000 / period;
      
      Serial.println("START OF BLADE");
      prevState = false;
    }
  }
  
  int getRPM() {
    return RPM;
  }


};