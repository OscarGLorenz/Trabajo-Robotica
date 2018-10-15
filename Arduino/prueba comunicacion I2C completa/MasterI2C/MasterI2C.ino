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
int sig = 1;

void setup() {
  initialize();
}

void I2C_Send (String code, int device) {
  Wire.beginTransmission(device);
  for (int i = 0; i < code.length(); i++)Wire.write(code[i]);
  delayMicroseconds(10);
  Wire.endTransmission();
}

//Realizar la request

void makeRequest() {
  Wire.requestFrom(M1_adress, 1);
  while (Wire.available()) {
    sig =(int) Wire.read();
  }
}

void loop() {
  if (analogRead(ENDSTOP_PIN) < 150) {
    I2C_Send(STOP, M1_adress);
    delay(10);
    makeRequest();
  }
  else {
    if (sig == 1) {
      I2C_Send("J3 100", M1_adress);
      sig = 0;
      
    }

    else {
      makeRequest();
      delay(10);
    }

  }


  Serial.println(sig);
}
