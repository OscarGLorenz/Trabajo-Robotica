#include <Wire.h>

void setup() {
  Wire.begin();         // Unirse al bus 12C
  Serial.begin(115200); // Iniciar la comunicación Serial
}

/*
  void serialSend (String code, int device) {

  Wire.beginTransmission(device);
  for (i = 0; i < code.length(); i++) {
    Wire.write((char) code[i]);
  }
  Wire.endTransmission();

  }*/

void loop() {
  // Si llega algo por serial, mandar el comando 0x01 y el ángulo
  Wire.beginTransmission(0x10); // transmit to device #8
  Wire.write("S3 2.65 ");             // sends one byte
  Wire.endTransmission();    // stop transmitting

  delay(1000);
  /*Wire.beginTransmission(0x10); // transmit to device #8
  Wire.write('D');             // sends one byte
  Wire.endTransmission();    // stop transmitting
  delay(1000);
*/
}
