#include <TinyWireS.h> 
#include <Servo.h>
/*
 * Debug.h
 *
 *  Created on: 19 sept. 2016
 *      Author: oscar
 *  
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_



/* DEBUG es una macro que muestra el valor y nombre de algún valor o variable
 * por Serial
 * Ejemplo:
 *   int x = 45;
 *   DEBUG(x)
 *   Por serial pondría "x = 45"
*/
#define DEBUG(x) Serial.println(String(#x) + " = " String(x));

/*
 *  BRK es una macro que imprime por Serial la linea en la que se encuentra
 *  muy útil para hacer debug y ver si el programa llega o no a cierto punto
 *  Ejemplo:
 *    1. if(true)
 *    2.   BRK
 *    3. else
 *    4.   BRK
 *  Por serial solo pondría "Code reached line No.2"
 */
#define BRK Serial.println(F("Reached Line #" + String(__LINE__)) + " File:" + String(__FILE__));




#endif /* SRC_DEBUG_H_ */
#define AD0 PB0
#define AD1 PA1
#define AD2 PA2
#define AD3 PA3
#define AD4 PB2
#define SERVO PB1

union Address {
  struct {
    unsigned b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
  } Bit;
  byte Byte = 0;  
} address;

union Angle {
  byte Byte[2];
  short a;
} angle;

Servo servo;

void setup(){
  address.Bit.b0 = digitalRead(AD0);
  address.Bit.b1 = digitalRead(AD1);
  address.Bit.b2 = digitalRead(AD2);
  address.Bit.b3 = digitalRead(AD3);
  address.Bit.b4 = digitalRead(AD4);

  TinyWireS.begin(address.Byte);
  servo.attach(SERVO);
}

void loop(){
  if (TinyWireS.available()){       
    angle.Byte[0] = TinyWireS.receive();     
    angle.Byte[1] = TinyWireS.receive();     
  }
  
  servo.write(angle.a);
}

