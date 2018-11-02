#define ENCODERINO 2    // COMPILACIÓN CONDICIONAL, 1,2 o 3. EN UN FUTURO A LA EEPROM
#define DEBUG   // Descomentar para generar datos extra pos serial USB

#include <SPI.h>
#include "Debug.h"

#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4 //Pin Dir del DRV8825
#define EN_PIN 6 //Pin enable del DRV8825



// Lee el encoder y devuelve en grados la posición
float leerEncoder()  {
  long angulo;

  // COMUNICACIÓN SPI
  digitalWrite(CSN_PIN, LOW);
  byte msb = SPI.transfer(0xFF);
  byte lsb = SPI.transfer(0xFF);
  digitalWrite(CSN_PIN, HIGH);

  angulo = (((msb << 8) | lsb) & 0X3FFF); // msb | lsb (usando 14 bit)

  return ((float) angulo) * 0.0219726;
}
float homeAngle;

// Diferencia entre ángulos (-180<phi<=180)
float difAngle(float a, float b) {
  double angulo = a - b;
  
  // DANGER DANGER DANGER LA ZONA MUERTA TIENE QUE SER UN PARAMETRO
  
  if (abs(angulo) < 1) return 0;
  else if (angulo < -180.0)
    return 360.0 + angulo;
  else if (angulo > 180.0)
    return -360.0 + angulo;
  return angulo;
}



// Obtener el ángulo teniendo en cuenta que se dan varias vueltas
float pastAngleRel = 0;
int nvueltas = 0;
float HomeAngle = 256;
float getAngle() {
  float angleRel = leerEncoder();

  if ((pastAngleRel >= 0 && pastAngleRel < 10) && (angleRel < 360 && angleRel > 350)) nvueltas += 1;
  else if ((pastAngleRel < 360 && pastAngleRel > 350) && (angleRel >= 0 && angleRel < 10)) nvueltas -= 1;

  pastAngleRel = angleRel;

  float fangle = angleRel;
  return (fangle + nvueltas * 360) / 5.0;
}


// Ctes PID
float Kp = 1.5;   

float speed = 0.0;// Velocidad objetivo

float ref = getAngle();    // Posición objetivo

void control_pos() {

  Kp = 1.5;

  float angle = getAngle();

  float error = ref - angle;
  //calcula la velocidad
  float PID = Kp * error;

  unsigned long dly = fabs(1.0 / (PID / 60.0 * 200.0 * 32.0 ) / 2.0) * 1e6;
  if (PID == 0) dly = 0;

  static unsigned long t = millis();
  while (millis() - t < 10) {
   
    if (dly == 0) continue;
    if (error <= 0) {
      digitalWrite(DIR_PIN, HIGH);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(dly);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(dly);
    } else {
      digitalWrite(DIR_PIN, LOW);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(dly);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(dly);
    }
  }
  t = millis();

}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // Serial
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.setTimeout(10);    
  #endif
  
  Serial1.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial1.setTimeout(10);    // Timeout de 5ms

  // SPI
  SPISettings settings(10000000, MSBFIRST, SPI_MODE1);  // Parámetros de la comunicación (Datasheet AS5047D)
  SPI.begin();                                          // Iniciar la comunicación
  delay(1000);                                          // TODO (Probar si es necesario)
  SPI.beginTransaction(settings);                       // Empezar comunicación SPI

  digitalWrite(LED_BUILTIN, LOW); // Apagar LED

  digitalWrite(EN_PIN, LOW);  // Encender Driver
  delay(1000);

}

bool homing = false;

void advance(float distance, float speedScrew) {

  long int nstep, n;
  n = 0;
  //una vuelta son 200 pasos y avanza 8 mm
  nstep = fabs((distance / 8.0) * 200);

  unsigned long dly;

  if (speedScrew == 0) dly = 0;

  // tengo que calcular la velocidad lineal ahora tengo velocidad angular
  dly = fabs((1.0 / (speedScrew / 60.0 * 200.0 ) / 2.0) * 1e6);

  while (n <= nstep) {
    // Si llega un ID=30 significa que se ha tocado el endstop
    // En el modo home se pasa a ir a after_home
    if (Serial1.available() > 0 &&  homing == true) {
      int id = Serial1.parseInt();
      Serial1.parseInt();
      
      if (id == 30) {
        homing = false;
        return;
      }
    }

    if (distance == 0) continue;
    if (distance > 0) {

      digitalWrite(DIR_PIN, HIGH);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(dly);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(dly);

    } else {

      digitalWrite(DIR_PIN, LOW);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(dly);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(dly);
    }
    n++;
  }

}
// Posiciones en el endstop
#if  ENCODERINO == 1

#define HOME_ANGLE 0.0
#define AFTER_HOME 110.0

#elif  ENCODERINO == 2

#define HOME_ANGLE 100.0
#define AFTER_HOME 45.0;

#elif  ENCODERINO == 3
#define HOME_ANGLE -120.0
#define AFTER_HOME -45.0;
#endif

float offset = 0;
void loop() {
 Serial.println(getAngle() + offset);
 // delay(100);
  
  // Si llega algo por el puerto serie
  if (Serial1.available()) {
    int id = Serial1.parseInt();      // Guardar el ID

    float q;                          // Argumento auxiliar
    switch (id) {
      case 0:                         // Ir al Home
      Serial1.parseInt();             // Purga el buffer
      
      #ifdef DEBUG
      Serial.println("HOME");
      #endif

      #if   ENCODERINO == 1           
        homing = true;                // Rutina home
        advance(-1000, 80);           // Ir a un extremo
        advance(AFTER_HOME_1, 150);   // Ir a after home
        ref = AFTER_HOME;
        
      #elif ENCODERINO == 2
        homing = true;
        advance(100000, 120);
        nvueltas = 0;
        HomeAngle = leerEncoder();
        offset = HOME_ANGLE - getAngle();
        ref = AFTER_HOME - offset;
                
      #elif ENCODERINO == 3

      #endif
        break;


      case 1:                        // Ir a posición 
        q = Serial1.parseFloat();    // Obtener posición
        Serial1.parseFloat();        // Purgar buffer

        #ifdef DEBUG
        Serial.print("POS "); 
        Serial.println(q);
        #endif DEBUG
        
        #if ENCODERINO == 1        
          advance(q-ref, 120);       // Ir al la posición   
          ref = q;                   // Guardar posición nueva
        #else
          ref = q - offset;
        #endif

        break;

      case 2:                        // Ir a velocidad
        q = Serial1.parseFloat();    // Obtener posición
        Serial1.parseFloat();        // Purgar buffer

        #ifdef DEBUG
        Serial.print("VEL "); 
        Serial.println(q);
        #endif DEBUG
        
        #if ENCODERINO == 1
          speed = q;
        #endif

        break;
        
      case 3:                        // Activar/Desactivar stepper
        Serial1.parseInt();          // Purgar Buffer

        // Toggle al pin del enable
        digitalWrite(EN_PIN, !digitalRead(EN_PIN));

        #ifdef DEBUG
        Serial.println("EN/DIS"); 
        #endif DEBUG
      break;
      
    }
  } // ... end switch

  #if ENCODERINO == 1   // Mover el eje lineal si la velocidad no es nula
    if (fabs(speed) > 0.1) {
      if (speed > 0)
        advance(0.1, fabs(speed));
      else
        advance(-0.1, fabs(speed));
    }
  #else
      control_pos();
  #endif

 // Mandar cada cierto tiempo la referencia actual. En un futuro que sea la posición actual
 static long last = millis();
 if ((millis() - last) > 500) {
   Serial1.println(ref);
   last = millis();
 }
}

