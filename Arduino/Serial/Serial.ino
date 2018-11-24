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
  static unsigned long eachWrite = millis();
  if (millis() - eachWrite > 200) {
          char cmd[40] = {0};                  // Se manda la posición con formato %3.3f
          
          char aux1[10];dtostrf(q1.evaluate(), 3, 3, aux1); 
          char aux2[10];dtostrf(q2.evaluate(), 3, 3, aux2); 
          char aux3[10];dtostrf(q3.evaluate(), 3, 3, aux3); 

          strcat(cmd,aux1);
          strcat(cmd," ");
          strcat(cmd,aux2);
          strcat(cmd," ");
          strcat(cmd,aux3);
          strcat(cmd," ");

          Serial.println(cmd);

          eachWrite = millis();
  }

  
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
          Serial.read();

          String str1 = Serial.readStringUntil('_');
          String str2 = Serial.readStringUntil('_');
          String str3 = Serial.readStringUntil('_');

          sp1.loadSpline(str1);
          sp2.loadSpline(str2);
          sp3.loadSpline(str3);
          break;
  
      }
    }
  }
}

SIGNAL(TIMER0_COMPA_vect) {
        if (millis()%100==1) {
            float now = (millis() - sp1.getStart())/1000.0;
            if (!sp1.stop()) q1.goPos(sp1.evaluate(now));
            if (!sp2.stop()) q2.goPos(sp2.evaluate(now));
            if (!sp3.stop()) q3.goPos(sp3.evaluate(now));
        }


            
            q1.update();
            q2.update();
            q3.update();
}
