class LinearActuator {
  private:
  int PowerPin;
  int GndPin;

  public:
  LinearActuator(int PowerPin, int GndPin) {
    this->PowerPin = PowerPin;
    this->GndPin = GndPin;
    
    pinMode(GndPin, OUTPUT);
    pinMode(PowerPin, OUTPUT);
  }

  void FullyExtend(){
    digitalWrite(PowerPin, HIGH);
    digitalWrite(GndPin, LOW);
  }
  void FullyRetract(){
    digitalWrite(PowerPin, LOW);
    digitalWrite(GndPin, HIGH);
  }
};