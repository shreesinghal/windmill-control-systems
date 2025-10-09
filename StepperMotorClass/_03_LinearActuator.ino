class LinearActuator {
  private:
    enum linActuatorState {
      Fully_Extended,
      Fully_Retracted
    };
    
    int redWirePin;
    int blackWirePin;    
    linActuatorState actuatorState;

  public:
    LinearActuator(int redWirePin, int blackWirePin) {
      this->redWirePin = redWirePin;
      this->blackWirePin = blackWirePin;

      pinMode(redWirePin, OUTPUT);
      pinMode(blackWirePin, OUTPUT);

      this->fullyRetract();
    }

    void fullyRetract() {
      digitalWrite(blackWirePin, HIGH);
      digitalWrite(redWirePin, LOW);

      actuatorState = Fully_Retracted;
    }

    void fullyExtend() {
      digitalWrite(blackWirePin, LOW);
      digitalWrite(redWirePin, HIGH);

      actuatorState = Fully_Extended;
    }

    linActuatorState getActuatorState() {
      return actuatorState;
    }
};