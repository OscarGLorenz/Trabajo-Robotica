#include <Wire.h>

#define ENDSTOP_PIN A0
#define M1_adress 0x11
#define M2_adress 0x12
#define M3_adress 0x13
#define STOP  "J30"

void initialize() {
  Wire.begin();         // Unirse al bus 12C
  Serial.begin(115200); // Iniciar la comunicación Serial
  pinMode(ENDSTOP_PIN, INPUT);

}

void setup() {
  initialize();
}

void SerialSend (String code, int device) {
  Wire.beginTransmission(device);
  for(int i=0;i<code.length();i++)Wire.write(code[i]);
  delayMicroseconds(10);
  Wire.endTransmission();
}

void loop() {
  if (analogRead(ENDSTOP_PIN) < 150)SerialSend(STOP, M1_adress);
  else {
    SerialSend("J3 100", M1_adress);
  }
}
