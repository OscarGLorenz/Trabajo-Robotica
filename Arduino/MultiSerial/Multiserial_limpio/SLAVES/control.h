#ifndef CONTROL_H_
#define CONTROL_H_
#include "Encoder.h"

#define ZM 1 //zona muerta

/* Posiciones en el endstop y posiciones despues del home */
#if  ENCODERINO == 1
#define HOME_ANGLE 0.0
#define AFTER_HOME 110.0
float offset = 0;
#elif  ENCODERINO == 2
#define HOME_ANGLE 117.0
#define AFTER_HOME 45.0;
float offset = 73 + 90;
#elif  ENCODERINO == 3
#define HOME_ANGLE -114.0
#define AFTER_HOME -45.0;
float offset = -120 + 30; //-100;
#endif

long int t = millis();
unsigned long dly = 0;
float angleRef, angleRead;
float error, Kp, PID;

float ref = 20;


float abs_angle;
float pastAngleRel = 0;
float HomeAngle = 0;
int nvueltas = 0;

// Diferencia entre ángulos (-180<phi<=180)
float difAngle(float a, float b) {
  double angulo = a - b;
  //zona muerta
  if (abs(angulo) < ZM) return 0;
  else if (angulo < -180.0)
    return 360.0 + angulo;
  else if (angulo > 180.0)
    return -360.0 + angulo;
  return angulo;
}

float getAngle() {
  #if ENCODERINO == 1
    float angleRel = encoder.leerEncoder();
  #elif ENCODERINO == 2
    float angleRel = encoder.leerEncoder();
  #elif ENCODERINO == 3
    float angleRel = 360 - encoder.leerEncoder();
  #endif

  if ((pastAngleRel >= 0 && pastAngleRel < 50) && (angleRel <= 360 && angleRel > 320)) 
    nvueltas -= 1;
  else if ((pastAngleRel <= 360 && pastAngleRel > 320) && (angleRel >= 0 && angleRel < 50)) 
    nvueltas += 1;

   pastAngleRel = angleRel;

    float fangle = (angleRel - HomeAngle) + nvueltas * 360;
    return (HOME_ANGLE + fangle / 5);
}



float speed = 0.0;// Velocidad objetivo

void control_pos() {

  Kp = 1.5;

  angleRead = getAngle();

  #if ENCODERINO == 2
    error = ref - angleRead;
  #elif ENCODERINO==3
    error = -ref + angleRead;
  #endif

  //calcula la velocidad
  PID = Kp * error;

  dly = fabs(1.0 / (PID / 60.0 * 200.0 * 32.0 ) / 2.0) * 1e6;
  if (PID == 0) dly = 0;

  t = millis();
  while (millis() - t < 10) {

    if (dly == 0) continue;

    bool cond;
  #if ENCODERINO == 1
    cond = (error <= 0);
  #elif ENCODERINO == 2
    cond = (error >= 0);
  #elif ENCODERINO == 3
    cond = (error >= 0);
  #endif

  if (cond) {
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
}

bool homing = false;
bool homeDonete = false;

void advance(float distance, float speedScrew) {

  long int nstep, n;
  n = 0;
  //una vuelta son 200 pasos y avanza 8 mm
  nstep = fabs((distance / 8.0) * 200);

  unsigned long dly;
  // tengo que calcular la velocidad lineal ahora tengo velocidad angular
  dly = fabs((1.0 / (speedScrew / 60.0 * 200.0 ) / 2.0) * 1e6);

#ifdef ENCODERINO == 1
  nstep *= 8;
  dly /= 8;
#endif

  if (fabs(speedScrew) < 10e-3) {
    dly = 0;
    nstep = -1;
  }

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
int id = 0;
int sentido;

#endif

