const int directionPin = 6, stepPin = 7;
const int PowerPin = 2, GndPin = 3;
StepperMotor *s;
Tachometer *t;
LinearActuator *l;

const int MH_SENSOR = 12;
bool prevState = false;
float RPM = 0, period = 0;
unsigned long startTime = 0;

void setup() {
  // instantiate stepper motor
  s = new StepperMotor(directionPin, stepPin);
  t = new Tachometer(MH_SENSOR);
  l = new LinearActuator(PowerPin, GndPin);

  // tachometer setup
  pinMode(MH_SENSOR, INPUT);

  // serial console setup
  Serial.begin(9600);
}
