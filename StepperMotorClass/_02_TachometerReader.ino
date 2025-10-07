class Tachometer {
  private:
    float RPM;
    int pin;
    bool prevState;
    long startTime;
    long period;
    int numBlades = 3;

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
    Serial.println(RPM/3);
  }
  
  float getRPM() {
    return RPM / numBlades;
  }

};