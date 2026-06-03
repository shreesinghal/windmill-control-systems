void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== Wind controller (pitch + MPPT + wind sensor) ===");

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_POT,     ADC_11db);
  analogSetPinAttenuation(PIN_I_SENSE, ADC_11db);
  analogSetPinAttenuation(PIN_V_SENSE, ADC_11db);

  ledcAttach(PIN_ENA, PWM_FREQ, PWM_RES);
  motorStop();

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000);

  // ---- TPS55288 ----
  if      (tpsProbe(TPS_ADDR_PRIMARY))   tpsAddr = TPS_ADDR_PRIMARY;
  else if (tpsProbe(TPS_ADDR_SECONDARY)) tpsAddr = TPS_ADDR_SECONDARY;
  else { Serial.println("WARN: TPS55288 not found — MPPT disabled"); mpptEnabled = false; }

  if (mpptEnabled) {
    Serial.printf("TPS55288 at 0x%02X\n", tpsAddr);
    delay(500);
    calibrateZeroCurrent();
    vout_cmd = VOUT_MIN_CMD;
    tpsSetVout(vout_cmd);
  }

  // ---- SDP8xx wind sensor ----
  sdp.begin(Wire, SDP8XX_I2C_ADDRESS_1);
  sdp.stopContinuousMeasurement();
  delay(50);
  uint16_t err = sdp.startContinuousMeasurementWithDiffPressureTCompAndAveraging();
  if (err) {
    char msg[64];
    errorToString(err, msg, sizeof(msg));
    Serial.printf("WARN: SDP sensor init failed: %s\n", msg);
    sdpOk = false;
  } else {
    Serial.println("SDP8xx OK");
    sdpOk = true;
  }

  Serial.println("Ready. CSV: ms,Vin,Iin,P,Vout,dir,wind,mode,setpoint");
}

// =============== Serial command parser ===============
void handleCommand(String buf) {
  buf.trim();
  if (buf.length() == 0) return;

  if      (buf.equalsIgnoreCase("e"))   { pitchState = PITCH_MANUAL; motorExtend();  Serial.println("Extending"); }
  else if (buf.equalsIgnoreCase("r"))   { pitchState = PITCH_MANUAL; motorRetract(); Serial.println("Retracting"); }
  else if (buf.equalsIgnoreCase("s"))   { pitchState = PITCH_IDLE;   motorStop();    Serial.println("Stopped"); }
  else if (buf.equalsIgnoreCase("p"))   { Serial.printf("pos=%.2f%% angle=%.2f deg raw=%d\n", readPosPercent(), currentAngleDeg(), readPosRaw()); }
  else if (buf.equalsIgnoreCase("cal")) startCalibration();
  else if (buf.equalsIgnoreCase("z"))   goToZeroRef();
  else if (buf.equalsIgnoreCase("op"))  goToOperatingAngle();
  else if (buf.equalsIgnoreCase("w"))   {
    Serial.printf("wind=%.2f m/s  dP=%.2f Pa  T=%.2f C  mode=%s  setpoint=%.2f W\n",
                  windSpeed_mps, lastDiffPressPa, lastTempC,
                  ctrlMode == MODE_MPPT ? "MPPT" : "HOLD", holdPowerSetpoint);
  }
  else if (buf.equalsIgnoreCase("mppt on"))  { mpptEnabled = true;  Serial.println("MPPT on"); }
  else if (buf.equalsIgnoreCase("mppt off")) { mpptEnabled = false; tpsSetVout(VOUT_MIN_CMD); Serial.println("MPPT off, Vout at min"); }
  else if (buf.equalsIgnoreCase("stat")) {
    Serial.printf("Vin=%.2f Iin=%.2f Vout=%.2f pitch=%.2f%% angle=%.2f deg zRef=%.2f%% wind=%.2f mode=%s\n",
                  readVin(), readIin(), vout_cmd,
                  readPosPercent(), currentAngleDeg(), zeroRefPercent,
                  windSpeed_mps, ctrlMode == MODE_MPPT ? "MPPT" : "HOLD");
  }
  else if (buf.startsWith("a "))    goToAngle(buf.substring(2).toFloat());
  else if (buf.startsWith("zset ")) { setZeroRef(buf.substring(5).toFloat()); Serial.printf("zeroRef = %.2f%%\n", zeroRefPercent); }
  else if (buf.startsWith("hold ")) { holdPowerSetpoint = buf.substring(5).toFloat(); Serial.printf("hold setpoint = %.2f W\n", holdPowerSetpoint); }
  else {
    float target = buf.toFloat();
    if (target >= 0 && target <= 100) startMoveTo(target);
    else Serial.println("?");
  }
}
