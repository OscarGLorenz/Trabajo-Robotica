#include <Wire.h>

void setup() {
  Wire.begin();         // Unirse al bus 12C
  Serial.begin(115200); // Iniciar la comunicación Serial
}

union Float {
  float raw;
  byte buffer[sizeof(float)];
};
Float angle;

void serialSend (String code, int device) {
  
  Wire.beginTransmission(device);
  for (i = 0; i < code.length(); i++) {
    Wire.write(code[i]);
  }
  Wire.endTransmission();

}

void loop() {
  // Si llega algo por serial, mandar el comando 0x01 y el ángulo
  if (Serial.available() > 1) {
    angle.raw = Serial.parseFloat();
    Wire.beginTransmission(0x11); // transmit to device #8
    Wire.write(0X01);
    Wire.write(angle.buffer, sizeof(float));             // sends one byte
    Wire.endTransmission();    // stop transmitting
  }

}
