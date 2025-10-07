// class PIDController {
//   private:
//     double idealRPM;  // Desired RPM
//     double currentRPM;
//     double lastRPM;
//     double error;
//     double integral;
//     double derivative;
    
//     double Kp, Ki, Kd = 0, 0, 0;
//     # kp is for response time
//     # ki is for maintaining the speed
//     # kd is amount of correction
//     unsigned long lastTime;

//   public:
//     PIDController(double Kp, double Ki, double Kd) {
//       this->Kp = Kp;
//       this->Ki = Ki;
//       this->Kd = Kd;
//       this->integral = 0;
//       this->lastRPM = 0;
//       this->error = 0;
//       this->derivative = 0;
//       this->lastTime = millis();
//     }

//     // Set desired RPM
//     void setSetpoint(double rpm) {
//       this->idealRPM = rpm;
//     }

//     // Update RPM and calculate control output
//     double update(double currentRPM) {
//       unsigned long currentTime = millis();
//       double elapsedTime = (currentTime - lastTime) / 1000.0;  // Convert time to seconds
      
//       // Calculate error
//       error = idealRPM - currentRPM;
      
//       // Calculate integral and derivative
//       integral += error * elapsedTime;
//       derivative = (currentRPM - lastRPM) / elapsedTime;

//       // Compute PID output
//       double output = Kp * error + Ki * integral + Kd * derivative;
      
//       // Save current RPM for next cycle
//       lastRPM = currentRPM;
//       lastTime = currentTime;
      
//       return output;
//     }
// };
