#include "Sim.h"
#include "Debug.h"
#include "spline.h"

NonDynamicSystem q1(20,0);
NonDynamicSystem q2(10,45);
NonDynamicSystem q3(10,-45);


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
    OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

}

Spline sp1;
Spline sp2;
Spline sp3;


void loop() {
  if (Serial.available() > 0) {
    char jcode = Serial.read();
    int motor;
    float q;
    if (jcode == 'J') {

      int cmd = Serial.parseInt();
      switch (cmd) {
        case 0:
        q1.goPos(110);
        q2.goPos(45);
        q3.goPos(-45);
        
        sp1.active = sp2.active = sp3.active = false;
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
          
          sp1.active = sp2.active = sp3.active = false;
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

          sp1.active = sp2.active = sp3.active = false;
          break;

        case 3:
          // Desactivar steppers
          break;
          
        case 5:
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();
          if (motor == 1) {
            sp1.loadSpline();
          } else if (motor == 2) {
            sp2.loadSpline();
          } else if (motor == 3) {
            sp3.loadSpline();
          }
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
        if (millis()%5==1) {
            if (sp1.active) q1.goPos(sp1.evaluate());
            if (sp2.active) q2.goPos(sp2.evaluate());
            if (sp3.active) q3.goPos(sp3.evaluate());
        }


            
            q1.update();
            q2.update();
            q3.update();
}
