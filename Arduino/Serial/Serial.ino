#include "Sim.h"
#include "Debug.h"
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
}

NonDynamicSystem q1(40);
NonDynamicSystem q2(5);
NonDynamicSystem q3(5);

void loop() {
  if (Serial.available() > 0) {
    char jcode = Serial.read();
    int motor;
    float q;
    if (jcode == 'J') {

      int cmd = Serial.parseInt();
      switch (cmd) {
        case 0:
          q1.goPos(0);
          q2.goPos(0);
          q3.goPos(0);
          break;

        case 1:
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
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
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
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
          Serial.println("D1 M" + String(motor) + " " + String(q));

          break;
      }
    }
  }
}
