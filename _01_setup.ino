const int directionPin = 7, stepPin = 6;
StepperMotor *s;

const int MH_SENSOR = 3;
bool prevState = false;
float RPM = 0, period = 0;
unsigned long startTime = 0;

void setup() {
  // instantiate stepper motor
  s = new StepperMotor(directionPin, stepPin);

  // tachometer setup
  pinMode(MH_SENSOR, INPUT);

  // serial console setup
  Serial.begin(9600);
}
