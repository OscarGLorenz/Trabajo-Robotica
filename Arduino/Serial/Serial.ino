
#include "Debug.h"
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
}

void loop() {
  if (Serial.available() > 0) {
    char jcode = Serial.read();
    int motor;
    float q;
    if (jcode == 'J') {

      int cmd = Serial.parseInt();
      switch (cmd) {
        case 0:
          // Hacer home
          break;

        case 1:
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt();
          q = Serial.parseFloat();
          // Mover motor a posición q ....

          break;

        case 2:
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt();
          q = Serial.parseFloat();
          // Mover motor a velocidad q ....

          break;

        case 3:
          // Desactivar steppers
          break;
        case 20:
          Serial.read(); Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt(); // Coger motor
          q = rand(); // Consultar por I2C motor y guardar en q

          Serial.println("D1 M" + String(motor) + " " + String(q));

          break;
      }
    }
  }
}
