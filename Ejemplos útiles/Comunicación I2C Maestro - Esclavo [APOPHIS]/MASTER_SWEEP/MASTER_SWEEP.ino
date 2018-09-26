#include <Wire.h>

#define HOME_POS (unsigned byte[]) {90,90,90,90,90,90,90,90,90,90,90,90}

float be1 = 25.0; float be2 = be1;
float w1 = 5.0; float w2 = w1;
float al1 = 0.0; float al2 = al1;
float k1 = 0; float k2 = k1;
float phi = M_PI;

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
  for (int i = 0b00000001; i <= 0b00001011; i++) {
        float t = millis()/1000.0;
        if (i%2)  write(i, al1 + be1 * sin( w1*t + k1*i)      -(HOME_POS[i]-90));
        else      write(i, al2 + be2 * sin( w2*t + k2*i -phi) -(HOME_POS[i]-90));
  }
}

