const int directionPin = 6, stepPin = 7;
const int MH_SENSOR = 12;

StepperMotor *pitchStepper;
Tachometer *tacho;

void setup() {
  // instantiate stepper motor
  pitchStepper = new StepperMotor(directionPin, stepPin);
  tacho = new Tachometer(MH_SENSOR);
  
  // l = new LinearActuator(PowerPin, GndPin);

  Serial.begin(9600);
}
