// =============== Pitch read ===============
int readPosRaw() {
  long sum = 0;
  for (int i = 0; i < 16; i++) sum += analogRead(PIN_POT);
  return sum / 16;
}
float readPosPercent() {
  int raw = readPosRaw();
  float pct = 100.0f * (raw - POT_RETRACTED) / (float)(POT_EXTENDED - POT_RETRACTED);
  if (pct < 0)   pct = 0;
  if (pct > 100) pct = 100;
  return pct;
}
float currentAngleDeg() {
  float frac = (readPosPercent() - zeroRefPercent) / 100.0f;
  if (frac < -1.0f) frac = -1.0f;
  if (frac >  1.0f) frac =  1.0f;
  return asinf(frac) * 180.0f / PI;
}

void startMoveTo(float targetPercent) {
  pitchTarget  = targetPercent;
  pitchState   = PITCH_MOVING;
  pitchStateT0 = millis();
}
void goToAngle(float angleDeg) {
  float pct = zeroRefPercent + sinf(angleDeg * PI / 180.0f) * 100.0f;
  if (pct < 0)   pct = 0;
  if (pct > 100) pct = 100;
  startMoveTo(pct);
}
void setZeroRef(float pct) { zeroRefPercent = pct; }
void goToZeroRef()         { startMoveTo(zeroRefPercent); }
void goToOperatingAngle() {
  float t = zeroRefPercent + 42.26f;
  if (t > 100) t = 100;
  startMoveTo(t);
}
void startCalibration() {
  motorRetract(200);
  pitchState   = PITCH_CAL_RETRACTING;
  pitchStateT0 = millis();
  Serial.println("CAL: retracting...");
}

void pitchTask() {
  static unsigned long t_last = 0;
  if (millis() - t_last < PITCH_TICK_MS) return;
  t_last = millis();

  switch (pitchState) {
    case PITCH_IDLE:
    case PITCH_MANUAL: break;

    case PITCH_MOVING: {
      float pos = readPosPercent();
      float err = pitchTarget - pos;
      if (fabs(err) < PITCH_TOLERANCE) {
        motorStop(); pitchState = PITCH_IDLE;
        Serial.printf("Reached %.2f%% (target %.2f%%)\n", pos, pitchTarget);
        break;
      }
      if (millis() - pitchStateT0 > PITCH_TIMEOUT) {
        motorStop(); pitchState = PITCH_IDLE;
        Serial.printf("Pitch timeout @ %.2f%%\n", pos);
        break;
      }
      int speed = constrain((int)(fabs(err) * 8 + 80), 80, 255);
      if (err > 0) motorExtend(speed); else motorRetract(speed);
      break;
    }
    case PITCH_CAL_RETRACTING:
      if (millis() - pitchStateT0 >= CAL_DRIVE_MS) {
        motorStop(); pitchState = PITCH_CAL_SETTLE_R; pitchStateT0 = millis();
      } break;
    case PITCH_CAL_SETTLE_R:
      if (millis() - pitchStateT0 >= CAL_SETTLE_MS) {
        POT_RETRACTED = readPosRaw();
        Serial.printf("CAL: POT_RETRACTED=%d\n", POT_RETRACTED);
        motorExtend(200);
        pitchState = PITCH_CAL_EXTENDING; pitchStateT0 = millis();
        Serial.println("CAL: extending...");
      } break;
    case PITCH_CAL_EXTENDING:
      if (millis() - pitchStateT0 >= CAL_DRIVE_MS) {
        motorStop(); pitchState = PITCH_CAL_SETTLE_E; pitchStateT0 = millis();
      } break;
    case PITCH_CAL_SETTLE_E:
      if (millis() - pitchStateT0 >= CAL_SETTLE_MS) {
        POT_EXTENDED = readPosRaw();
        Serial.printf("CAL: POT_EXTENDED=%d\n", POT_EXTENDED);
        Serial.printf("CAL: done. retracted=%d extended=%d\n", POT_RETRACTED, POT_EXTENDED);
        pitchState = PITCH_IDLE;
      } break;
  }
}
