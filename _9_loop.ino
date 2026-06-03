void loop() {
  static String buf = "";
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') { handleCommand(buf); buf = ""; }
    else                         { buf += c; }
  }
  pitchTask();
  windTask();
  mpptTask();
}