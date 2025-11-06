// Pins
const int stepperDirPin = 6, stepperStepPin = 7;
const int MH_SENSOR = 3;  // NOTE: this must be an interrupt compatible pin (on the Nano Every, that means pins 2 or 3)
const int actRedWire = 8, actBlackWire = 9;

StepperMotor *pitchStepper;
Tachometer *tacho;
LinearActuator *linAct;

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // instantiate stepper motor
  pitchStepper = new StepperMotor(stepperDirPin, stepperStepPin);
  tacho = new Tachometer(MH_SENSOR);
  linAct = new LinearActuator(actRedWire, actBlackWire);
  
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}
