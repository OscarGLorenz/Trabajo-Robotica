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

void loop() {
  // Si llega algo por serial, mandar el comando 0x01 y el ángulo
  if (Serial.available() > 1) {
    angle.raw = Serial.parseFloat();
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(0X01);
    Wire.write(angle.buffer,sizeof(float));              // sends one byte
    Wire.endTransmission();    // stop transmitting
  }

}
