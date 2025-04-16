const int directionPin = 6, stepPin = 7;
StepperMotor *s;
Tachometer *t;

const int MH_SENSOR = 12;
bool prevState = false;
float RPM = 0, period = 0;
unsigned long startTime = 0;

void setup() {
  // instantiate stepper motor
  s = new StepperMotor(directionPin, stepPin);
  t = new Tachometer(MH_SENSOR);

  // tachometer setup
  pinMode(MH_SENSOR, INPUT);

  // serial console setup
  Serial.begin(9600);
}
