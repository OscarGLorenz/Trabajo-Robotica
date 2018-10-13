#include <Wire.h>
#include "Sim.h"
#include "Debug.h"
NonDynamicSystem q1(20, 0);
NonDynamicSystem q2(2, M_PI_4);
NonDynamicSystem q3(2, -M_PI_4);

#define ENDSTOP_PIN A0
#define M1_address 0x11
#define M2_address 0x12
#define M3_address 0x13
#define STOP  "J30"
#define HOME "J0"
#define ANGLE "J1"
#define VEL "J2"
#define DISABLE "J3"
#define REQUEST "J20"


void initialize() {
  Wire.begin();         // Unirse al bus 12C
  Serial.setTimeout(10);
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  Serial.begin(115200); // Iniciar la comunicación Serial
  pinMode(ENDSTOP_PIN, INPUT);

}

void setup() {
  initialize();
}

void SerialSend (String code, int device) {
  Wire.beginTransmission(device);
  for (int i = 0; i < code.length(); i++)Wire.write(code[i]);
  delayMicroseconds(10);
  Wire.endTransmission();
}

//Para recibir la posicion con REQUEST, el maestro ahora es esclavo y el esclavo es maestro
/*
  void SerialReceive (int device) {
  char pos[Wire.available()];
  int i=0;
  while (1 < Wire.available()) {
    pos[i] = Wire.read();
    i++;
  }
  pos[i+1]="\0"
  return pos;
*/
void loop() {
  if (analogRead(ENDSTOP_PIN) < 150)SerialSend(STOP, M1_address);
  else if (Serial.available() > 0) {
    char jcode = Serial.read();
    int motor;
    float q;
    if (jcode == 'J') {
      int cmd = Serial.parseInt();
      switch (cmd) {
        case 0:
          motor = Serial.parseInt();
          if (motor == 1) {
            SerialSend(HOME, M1_address);
          } else if (motor == 2) {
            SerialSend(HOME, M2_address);
          } else if (motor == 3) {
            SerialSend(HOME, M3_address);
          }
          break;

        case 1:
          Serial.read();  Serial.read();// Eliminar " M" del buffer
          motor = Serial.parseInt();
          q = Serial.parseFloat();
          if (motor == 1) {
            q1.goPos(q);
            //SerialSend(ANGLE, M1_address);
            //SerialSend(q, M1_address);
          } else if (motor == 2) {
            q2.goPos(q);
            //SerialSend(ANGLE, M2_address);
            //SerialSend(q, M2_address);
          } else if (motor == 3) {
            q3.goPos(q);
            //SerialSend(ANGLE, M3_address);
            //SerialSend(q, M3_address);
          }
          break;

        case 2:
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();
          q = Serial.parseFloat();
          if (motor == 1) {
            q1.speed(q);
            //SerialSend(VEL, M1_address);
            //SerialSend(q, M1_address);
          } else if (motor == 2) {
            q2.speed(q);
            //SerialSend(VEL, M2_address);
            //SerialSend(q, M2_address);
          } else if (motor == 3) {
            q3.speed(q);
            //SerialSend(VEL, M3_address);
            //SerialSend(q, M3_address);
          }
          break;

        case 3: //disable stepper
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();
          if (motor == 1) {
            SerialSend(DISABLE, M1_address);
          } else if (motor == 2) {
            SerialSend(DISABLE, M2_address);
          } else if (motor == 3) {
            SerialSend(DISABLE, M3_address);
          }
          break;
        case 20:
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt(); // Coger motor
          //Serial.print("D1 M"); Serial.print(motor); Serial.print(" ");
          if (motor == 1) {
            q = q1.evaluate();
            //SerialSend(REQUEST, M1_address);
            //Serial.println(SerialReceive(M1_address));
          } else if (motor == 2) {
            q = q2.evaluate();
            //SerialSend(REQUEST, M2_address);
            //Serial.println(SerialReceive(M1_address));
          } else if (motor == 3) {
            q = q3.evaluate();
            //SerialSend(REQUEST, M3_address);
            //Serial.println(SerialReceive(M1_address));
          }
          char s[30];
          Serial.print("D1 M"); Serial.print(motor); Serial.print(" "); Serial.println(dtostrf(q, 3, 3, s));
          break;
      }
    }
  }
}

SIGNAL(TIMER0_COMPA_vect) {
  q1.update();
  q2.update();
  q3.update();

}
}
