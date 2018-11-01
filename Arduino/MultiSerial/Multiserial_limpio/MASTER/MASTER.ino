#include "ENCODERINO.h"  // Clase Encoderino

//#define DEBUG   // Descomentar para generar datos extra pos serial USB, INCOMPATIBLE CON MATLAB

// Referencia a serial y pin del endstop
Encoderino encoder1(&Serial1, A1); 
Encoderino encoder2(&Serial2, A2);
Encoderino encoder3(&Serial3, A3);

// Array para indexar
Encoderino * encoders[] = {&encoder1, &encoder2, &encoder3};

void setup() {
  // Comunicación USB
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  // Iniciar encoderinos
  encoders[0]->init();
  encoders[1]->init();
  encoders[2]->init();
}

void loop() {
  delay(50);  // DANGER DANGER DANGER DANGER OJO BLOQUEO PELIGROSO

  // Actualiza encoders, revisa si han chocado con el endstop y si se ha preguntado la posición desde MATLAB
  encoders[0]->update();
  encoders[1]->update();
  encoders[2]->update();

  // Si llegan ordenes de MATLAB
  if (Serial.available()) {
    char jcode = Serial.read();          // Guarda la J o lo que sea
    int motor;                           // ID del motor
    float q;                             // Argumento adicional
    
    if (jcode == 'J') {                  // Si es un comando J...

      int cmd = Serial.parseInt();       // Obtener el ID del comando
      switch (cmd) {
        
        case 0:                          // Comando de Home
          encoders[0]->goHome();         // Manda a los motores orden de Home
          encoders[1]->goHome();
          encoders[2]->goHome();

          #ifdef DEBUG                   // Debug muestra motor y orden
          Serial.print(motor);      
          Serial.println(": 0");
          #endif
          break;


        case 1:                          // Comando de posicion
          Serial.read();  Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Guarda el motor al que va destinada la orden
          q = Serial.parseFloat();       // Guarda la posición a la que debe ir el motor

          encoders[motor-1]->goPos(q);   // Manda el motor correspondiente a la posición
              
          #ifdef DEBUG                   // Debug muestra motor y orden
          Serial.print(motor); 
          Serial.print(": 1 "); 
          Serial.println(q);
          #endif
          break;


        case 2:                          // Comando de velocidad
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Guarda el motor al que va destinada la orden
          q = Serial.parseFloat();       // Guarda la posición a la que debe ir el motor

          encoders[motor-1]->speed(q);   // Manda el motor correspondiente a una velocidad
          
          #ifdef DEBUG                   // Debug muestra motor y orden
          Serial.print(motor);            
          Serial.print(": 2 "); 
          Serial.println(q);
          #endif
          break;


        case 3:                          // Comando de desactivar los stepper 
          encoders[0]->disable();
          encoders[1]->disable();
          encoders[2]->disable();
          
          #ifdef DEBUG                   // Debug muestra motor y orden
          Serial.print(motor); 
          Serial.println(": 3");
          #endif
          break;


        case 20:                         // Comando para pedir posición
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Coger motor

          q = encoders[motor-1]->getPos(); // Obtención de la posición
          
          char str[30];                  // Se manda la posición con formato %3.3f
          Serial.print("D1 M"); 
          Serial.print(motor); 
          Serial.print(" "); 
          Serial.println(dtostrf(q, 3, 3, str));
          break;
          
      }
    }
  }
}

