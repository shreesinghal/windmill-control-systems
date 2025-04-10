class LinearActuator {
  private:
  int PowerPin;
  int GndPin;

  public:
  LinearActuator(int PowerPin, int GndPin) {
    this->PowerPin = PowerPin;
    this->GndPin = GndPin;
  }

  void FullyExtend(){
    pinMode(PowerPin, HIGH);
    pinMode(GndPin, LOW);
  }
  void FullyRetract(){
    pinMode(PowerPin, LOW);
    pinMode(GndPin, HIGH);
  }
}