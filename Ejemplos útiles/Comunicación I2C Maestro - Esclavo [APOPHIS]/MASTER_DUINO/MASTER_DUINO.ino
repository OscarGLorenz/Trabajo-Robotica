#include<Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  if (Serial.available() >= 2) {
    byte buffer[2];
    Serial.readBytes(buffer, 2);
    Wire.beginTransmission(buffer[0]);
    Wire.write(buffer[1]);
    Wire.endTransmission();
  }
}
