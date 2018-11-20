#define ENCODERINO 3  // COMPILACIÓN CONDICIONAL, 1,2 o 3. EN UN FUTURO A LA EEPROM
#define LOBOTOMIA LOW

#include "spline.h"
#include "control.h"
#include "Encoder.h"

void setup() {
  encoder.init();
  pinMode(13,OUTPUT);
  Serial1.begin(115200);     // Comunicación Serial a 115200 Baudios
  Serial1.setTimeout(30);    // Timeout de 5ms

  encoder.enable(LOBOTOMIA);
}

void loop() {
  // Si llega algo por el puerto serie
  if (Serial1.available()) {
    id = Serial1.parseInt();      // Guardar el ID

    float q;                          // Argumento auxiliar
    switch (id) {
      /* RUTINA DE HOMING */
      case 50:
        Serial1.parseInt();             // Purga el buffer
        spline.active = false;
	
	#if   ENCODERINO == 1
	  homing = true;                // Rutina home
          advance(-1000, 150);           // Ir a un extremo
          advance(AFTER_HOME, 300);   // Ir a after home
          ref = AFTER_HOME;
          nvueltas = 0;
	#elif ENCODERINO == 2
          homing = true;
          advance(100000, 4 * 32);
          nvueltas = 0;
          HomeAngle = encoder.leerEncoder();
          ref = AFTER_HOME;
        #elif ENCODERINO == 3
          homing = true;
          delay(2000);
          advance(100000, 2 * 32);
          nvueltas = 0;
          HomeAngle = 360 - encoder.leerEncoder();
          ref = AFTER_HOME;
        #endif
        homeDonete = true;

        break;
      /* RUTINA DE HOMING */



      /* RUTINA DE POSICIONAMIENTO SIN INTERPOLACIÓN */
      case 1:
        spline.active = false;
        // Ir a posición
        q = Serial1.parseFloat();    // Obtener posición
        Serial1.parseFloat();        // Purgar buffer


	#if ENCODERINO == 1
          advance(q - ref, 120);     // Ir al la posición
          ref = q;                   // Guardar posición nueva
	#else
          ref = q;
	#endif

        break;
      /* RUTINA DE POSICIONAMIENTO SIN INTERPOLACIÓN */



      /* VELOCIDAD CONSTANTE SIN CONTROL*/
      case 2:                        // Ir a velocidad
        spline.active = false;

        q = Serial1.parseFloat();    // Obtener posición
        Serial1.parseFloat();        // Purgar buffer

        speed = q;
        if (q > 0)  sentido = 1;
        else if (q < 0) sentido = -1;
        else sentido = 0;

        break;
      /* VELOCIDAD CONSTANTE SIN CONTROL*/



      /* DESHABILITAR STEPPER */
      case 3:                        // Activar/Desactivar stepper
        Serial1.parseInt();          // Purgar Buffer
        spline.active = false;

        // Toggle al pin del enable
        encoder.toggleEnable();

        break;
      /* DESHABILITAR STEPPER */


	
      /* INTERPOLACIÓN POR SPLINES*/
      case 5:
        spline.loadSpline();
	      break;
      /* INTERPOLACIÓN POR SPLINES*/

    } // ... end switch
  }// ... end if serial  available 

   
  /* CONTROL DE VELOCIDAD Y HOMING */
  #if ENCODERINO == 1
    if (fabs(speed) > 0.1 && id == 2) {
      advance(sentido * 1, fabs(speed) * 60.0 / 8.0);
      ref += sentido * 1;
    }
  #elif ENCODERINO ==2
    if (homeDonete && id != 2)
      control_pos();
    else if (homeDonete) {
      advance((sentido * 1), fabs(speed) * 5 * 32 / 6);
      getAngle();
    }
  #elif ENCODERINO == 3
    if (homeDonete && id != 2)
      control_pos();
    else if (homeDonete && id == 2) {
      advance(-(sentido * 1), fabs( speed) * 5 * 32 / 6);
      getAngle();
    }
  #endif
  /* CONTROL DE VELOCIDAD Y HOMING */

  static float dt=0.01;
  /* ACTUALIZAR REFERENCIA CON LAS SPLINES */	
  if (!spline.stop(millis()/1000.0+dt)) {
    
  #if ENCODERINO == 1
      ref = advanceSpline(dt);   
     
      // Ir al la posición
  #else
      digitalWrite(13,!digitalRead(13));
      
      ref = spline.evaluate((millis()-spline.getStart())/1000.0);
  
  
  #endif
  }

  // Mandar cada cierto tiempo la referencia actual
  static long last = millis();
  if ((millis() - last) > 100) {
  #if ENCODERINO == 1
    Serial1.println(ref);
  #else
    Serial1.println(getAngle());
    //Serial1.println(ref);
    
  #endif
    last = millis();
  }
}
