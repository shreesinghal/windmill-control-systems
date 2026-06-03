

// =============== MPPT helpers ===============
float readAdcVolts(int pin, int samples = 32) {
  uint32_t acc = 0;
  for (int i = 0; i < samples; i++) acc += analogRead(pin);
  return ((float)acc / samples / ADC_MAX_COUNT) * ADC_VREF;
}
float readVin() { return readAdcVolts(PIN_V_SENSE) * V_DIVIDER_RATIO; }
float readIin() {
  float v = readAdcVolts(PIN_I_SENSE);
  return (v * ACS_DIVIDER - acs_zero_v * ACS_DIVIDER) / ACS_SENS_VPA;
}
void calibrateZeroCurrent() {
  float v = readAdcVolts(PIN_I_SENSE, 256);
  acs_zero_v = v;
  Serial.printf("ACS712 zero: %.4f V (post-divider)\n", v);
}
bool tpsProbe(int addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}
bool tpsWriteReg(int reg, int val) {
  Wire.beginTransmission(tpsAddr);
  Wire.write(reg); Wire.write(val);
  return Wire.endTransmission() == 0;
}
uint16_t voutToRefCode(float vout) {
  float vref_mv = (vout / FB_GAIN) * 1000.0f;
  if (vref_mv < VREF_OFFSET_MV) vref_mv = VREF_OFFSET_MV;
  float code = (vref_mv - VREF_OFFSET_MV) / VREF_LSB_MV;
  if (code < 0) code = 0;
  if (code > VREF_MAX_CODE) code = VREF_MAX_CODE;
  return (uint16_t)(code + 0.5f);
}
void tpsSetVout(float vout) {
  uint16_t code = voutToRefCode(vout);
  tpsWriteReg(0x00, code & 0xFF);
  tpsWriteReg(0x01, (code >> 8) & 0x03);
}

// =============== Wind task ===============
void windTask() {
  static unsigned long t_last = 0;
  if (millis() - t_last < WIND_PERIOD_MS) return;
  t_last = millis();

  if (sdpOk) {
    float dPa, tempC;
    uint16_t err = sdp.readMeasurement(dPa, tempC);
    if (!err) {
      lastDiffPressPa = dPa;
      lastTempC       = tempC;
      windSpeed_mps   = computeWindFromSensor(dPa, tempC);
    }
    // on error, keep last reading
  }

  // Mode switching with hysteresis (use absolute wind speed)
  float absWind = fabsf(windSpeed_mps);
  if (ctrlMode == MODE_MPPT && absWind > WIND_THRESH_HIGH) {
    float vin = readVin();
    float iin = readIin(); if (iin < 0) iin = 0;
    holdPowerSetpoint = vin * iin;
    ctrlMode = MODE_HOLD;
    Serial.printf(">> MODE: HOLD  setpoint=%.2f W  wind=%.2f m/s\n",
                  holdPowerSetpoint, windSpeed_mps);
  }
  else if (ctrlMode == MODE_HOLD && absWind < WIND_THRESH_LOW) {
    ctrlMode = MODE_MPPT;
    p_prev = 0; step_dir = +1;
    Serial.printf(">> MODE: MPPT  wind=%.2f m/s\n", windSpeed_mps);
  }
}

// =============== Combined MPPT / hold task ===============
void mpptTask() {
  static unsigned long t_last = 0;
  if (!mpptEnabled) return;
  if (millis() - t_last < MPPT_PERIOD_MS) return;
  t_last = millis();

  float vin = readVin();
  float iin = readIin();
  if (iin < 0) iin = 0;
  float p = vin * iin;

  if (iin > IIN_HARD_MAX) {
    vout_cmd -= 2.0f * vout_step;
    if (vout_cmd < VOUT_MIN_CMD) vout_cmd = VOUT_MIN_CMD;
    tpsSetVout(vout_cmd);
    Serial.println("!! OVERCURRENT — backing off");
    p_prev = 0;
    return;
  }

  if (vin < VIN_MIN_RUN) {
    vout_cmd = VOUT_MIN_CMD;
    tpsSetVout(vout_cmd);
    p_prev = 0;
    return;
  }

  if (ctrlMode == MODE_MPPT) {
    if (p < p_prev) step_dir = -step_dir;
    vout_cmd += step_dir * vout_step;
    if (vout_cmd > VOUT_MAX_CMD) { vout_cmd = VOUT_MAX_CMD; step_dir = -1; }
    if (vout_cmd < VOUT_MIN_CMD) { vout_cmd = VOUT_MIN_CMD; step_dir = +1; }
    p_prev = p;
  } else {
    // Hold power constant. Assumes raising vout_cmd off MPP reduces extracted power.
    float err = p - holdPowerSetpoint;
    if (fabs(err) > HOLD_TOLERANCE) {
      if (err > 0) vout_cmd += vout_step;   // too much power -> shed
      else         vout_cmd -= vout_step;   // not enough -> draw more
      if (vout_cmd > VOUT_MAX_CMD) vout_cmd = VOUT_MAX_CMD;
      if (vout_cmd < VOUT_MIN_CMD) vout_cmd = VOUT_MIN_CMD;
    }
  }

  tpsSetVout(vout_cmd);

  // CSV: ms,Vin,Iin,P,Vout,dir,wind,mode,setpoint
  Serial.print(millis());            Serial.print(',');
  Serial.print(vin, 3);              Serial.print(',');
  Serial.print(iin, 3);              Serial.print(',');
  Serial.print(p, 3);                Serial.print(',');
  Serial.print(vout_cmd, 3);         Serial.print(',');
  Serial.print(step_dir);            Serial.print(',');
  Serial.print(windSpeed_mps, 2);    Serial.print(',');
  Serial.print(ctrlMode == MODE_MPPT ? "MPPT" : "HOLD"); Serial.print(',');
  Serial.println(holdPowerSetpoint, 2);
}