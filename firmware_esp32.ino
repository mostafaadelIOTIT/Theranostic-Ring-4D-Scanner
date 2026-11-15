#include <Arduino.h>
#include "mbedtls/md.h"

const int STEP_PIN = 18;
const int DIR_PIN = 19;
const int THERMAL_PIN = 34;
const int NIR_SPECTRA_PIN = 35;
const int LLLT_LASER_PIN = 2;
const int UVC_LED_PIN = 4;
const float BETA = 3950;
int currentAngle = 0;

String signTelemetrySHA256(String payload) {
  byte shaResult[32];
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) payload.c_str(), payload.length());
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  String hashStr = "";
  for (int i = 0; i < 16; i++) {
    char hex[3];
    sprintf(hex, "%02x", (int)shaResult[i]);
    hashStr += hex;
  }
  return hashStr;
}

void rotateRingGantry(int steps, bool clockwise) {
  digitalWrite(DIR_PIN, clockwise ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1200);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1200);
  }
}

float readTissueTemperature() {
  int analogValue = analogRead(THERMAL_PIN);
  return 1 / (log(1 / (4095.0 / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
}

void setup() {
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(LLLT_LASER_PIN, OUTPUT);
  pinMode(UVC_LED_PIN, OUTPUT);
  Serial.println("[THERANOSTIC-RING] 4D Limb Scanner & Edge SHA-256 Security: ONLINE");
}

void loop() {
  rotateRingGantry(17, true);
  currentAngle = (currentAngle + 30) % 360;

  float tissueTempC = readTissueTemperature();
  int fungalSpectralIndex = map(analogRead(NIR_SPECTRA_PIN), 0, 4095, 0, 100);

  bool inflammationDetected = (tissueTempC > 35.5 || tissueTempC < 29.0);
  bool fungalColonyDetected = (fungalSpectralIndex > 65);

  String clinicalStatus = "NORMAL_PERFUSION";
  String therapyAction = "NONE";

  if (inflammationDetected || fungalColonyDetected) {
    Serial.println(">>> [PATHOLOGY LOCK] Angle: " + String(currentAngle) + " deg");
    if (inflammationDetected) {
      clinicalStatus = (tissueTempC > 35.5) ? "DEEP_INFLAMMATION" : "ISCHEMIA_RISK";
      digitalWrite(LLLT_LASER_PIN, HIGH);
      therapyAction = "LLLT_ACTIVE";
    }
    if (fungalColonyDetected) {
      clinicalStatus += "|FUNGAL_POS";
      digitalWrite(UVC_LED_PIN, HIGH);
      therapyAction += "+UVC_ACTIVE";
    }
    delay(1500);
    digitalWrite(LLLT_LASER_PIN, LOW);
    digitalWrite(UVC_LED_PIN, LOW);
  }

  String payload = "{\"angle_deg\":" + String(currentAngle) +
                   ",\"temp_c\":" + String(tissueTempC, 1) +
                   ",\"nir_fungal_idx\":" + String(fungalSpectralIndex) +
                   ",\"diagnosis\":\"" + clinicalStatus + "\"" +
                   ",\"therapy\":\"" + therapyAction + "\"}";

  Serial.println("[SCAN-4D] " + payload);
  Serial.println("[SHA256-SIG] " + signTelemetrySHA256(payload));
  delay(800);
}
