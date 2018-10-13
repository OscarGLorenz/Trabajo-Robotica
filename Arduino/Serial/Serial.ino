#include "Sim.h"
#include "Debug.h"
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
    OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
}


NonDynamicSystem q1(20);
NonDynamicSystem q2(2);
NonDynamicSystem q3(2);

void loop() {
  if (Serial.available() > 0) {
    char jcode = Serial.read();
    int motor;
    float q;
    if (jcode == 'J') {

      int cmd = Serial.parseInt();
      switch (cmd) {
        case 0:
          break;

        case 1:
         Serial.read();  Serial.read();// Eliminar " M" del buffer
          motor = Serial.parseInt();
          q = Serial.parseFloat();
          if (motor == 1) {
            q1.goPos(q);
          } else if (motor == 2) {
            q2.goPos(q);
          } else if (motor == 3) {
            q3.goPos(q);
          }

          break;

        case 2:
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();
          q = Serial.parseFloat();
          if (motor == 1) {
            q1.speed(q);
          } else if (motor == 2) {
            q2.speed(q);
          } else if (motor == 3) {
            q3.speed(q);
          }
          break;

        case 3:
          // Desactivar steppers
          break;
        case 20:
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt(); // Coger motor
          if (motor == 1) {
            q = q1.evaluate();
          } else if (motor == 2) {
            q = q2.evaluate();
          } else if (motor == 3) {
            q = q3.evaluate();
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
