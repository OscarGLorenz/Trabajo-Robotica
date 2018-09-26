#include <Wire.h>

void write(byte dir, byte angle) {
    Wire.beginTransmission(dir);
    Wire.write(angle);
    Wire.endTransmission(); 
}

void setup() {
  Wire.begin();
  Serial.begin(115200);                        
}


void loop() {
  if (Serial.available()) {
    byte dir = Serial.parseInt();
    byte pos = Serial.parseInt();
    Serial.println("Address: " + String(dir) + " Angle: " + String(pos) + " ");
  }
}

