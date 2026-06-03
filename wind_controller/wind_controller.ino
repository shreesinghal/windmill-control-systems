// ==============================================================
//  Integrated Wind Turbine Controller
//    - Blade pitch (linear actuator + pot feedback, L298N driver)
//    - MPPT (TPS55288 over I2C, ACS712 current + voltage divider)
//    - Windspeed sensor (Sensirion SDP8xx)
//    - Mode switching: MPPT below 10 m/s, hold-power above
//
//  Pin map (ESP32):
//    Pitch:   IN1=25, IN2=26, ENA=27(PWM), POT=33
//    MPPT:    SDA=21, SCL=22, I_SENSE=34, V_SENSE=35
//    SDP8xx:  shares I2C bus (SDA=21, SCL=22)
//
//  Libraries required:
//    SensirionI2CSdp  (install via Arduino Library Manager)
// ==============================================================

#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <SensirionI2CSdp.h>

SensirionI2CSdp sdp;
