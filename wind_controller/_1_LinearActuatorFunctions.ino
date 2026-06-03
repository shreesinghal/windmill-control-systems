// =============== Motor primitives ===============
void motorStop()                  { digitalWrite(PIN_IN1, LOW);  digitalWrite(PIN_IN2, LOW);  ledcWrite(PIN_ENA, 0); }
void motorExtend(int speed = 255) { digitalWrite(PIN_IN1, HIGH); digitalWrite(PIN_IN2, LOW);  ledcWrite(PIN_ENA, speed); }
void motorRetract(int speed = 255){ digitalWrite(PIN_IN1, LOW);  digitalWrite(PIN_IN2, HIGH); ledcWrite(PIN_ENA, speed); }
