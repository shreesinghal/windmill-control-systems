// these are global variables so that it can be accessed by static methods
volatile unsigned long period = 0;
volatile unsigned long lastTime = 0;

class Tachometer {
  private:
    const int numBlades = 3;

  public:
    Tachometer(int mhSensorPin) {
      pinMode(mhSensorPin, INPUT);

      // This attaches an interupt to the sensor pin. 
      //   So, whenever the pin reads HIGH to LOW (aka falling),
      //   it will automatically run the calculatePeriod function to calculate the period
      attachInterrupt(digitalPinToInterrupt(mhSensorPin), calculatePeriod, FALLING);
    }

    float getRPM() {
      noInterrupts();             // prevent update while reading
      unsigned long p = period;
      interrupts();

      if (p > 0) {    // avoid div by 0
        float rpm = (60000.0 / p) / numBlades;
        return rpm;
      } else {
        return 0.0;
      }
    }

    // this is an ISR Interupt Service Routine Function
    static void calculatePeriod() {
      unsigned long now = millis();
      period = now - lastTime;
      lastTime = now;
    }
};