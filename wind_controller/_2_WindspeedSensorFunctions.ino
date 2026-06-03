// =============== Windspeed sensor (SDP8xx) ===============
const float R_SPECIFIC  = 287.05f;
const float P_STATIC_PA = 101325.0f;
const unsigned long WIND_PERIOD_MS = 500;

float windSpeed_mps   = 0.0f;
float lastDiffPressPa = 0.0f;
float lastTempC       = 25.0f;
bool  sdpOk           = false;

float computeWindFromSensor(float dPa, float tempC) {
  float tempK = tempC + 273.15f;
  float rho   = P_STATIC_PA / (R_SPECIFIC * tempK);
  if (dPa > 0) return  sqrtf((2.0f * dPa)        / rho);
  if (dPa < 0) return -sqrtf((2.0f * fabsf(dPa)) / rho);
  return 0.0f;
}
