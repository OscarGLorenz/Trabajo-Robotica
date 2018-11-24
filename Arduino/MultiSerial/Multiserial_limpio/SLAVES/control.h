#ifndef CONTROL_H_
#define CONTROL_H_

// Espera sin bloqueo, uso: NOBLOCK_DELAY(sentencias,tiempo en ms)
#define CONCAT(x,y) x ## y
#define EVAL(x,y)  CONCAT(x,y)
#define DELAY_VAR  EVAL(DELAY,LINE)
#define NOBLOCK_DELAY(f,t) static long DELAY_VAR = millis(); if (millis() - DELAY_VAR  > t) { f DELAY_VAR = millis();}
#define DEBUG(x) Serial1.println(String(#x) + " = " + String(x));


#include "Encoder.h"

#define ZM 0.5 //zona muerta

/* Posiciones en el endstop y posiciones despues del home */
#if  ENCODERINO == 1
#define HOME_ANGLE 0.0
#define AFTER_HOME 110.0
float offset = 0;
#elif  ENCODERINO == 2
#define HOME_ANGLE 103.0
#define AFTER_HOME 45.0;
float offset = 73 + 90;
#elif  ENCODERINO == 3
#define HOME_ANGLE 15.0
#define AFTER_HOME -45.0;
float offset = -120 + 30; //-100;
#endif

long int t = millis();
long int t_ant = millis();

unsigned long dly = 0;
float angleRef, angleRead;

float  PID, error;

#ifdef ENCODERINO == 2
float Kp_1 = 4 , Kff = 0.8 , Kd = 0.0, Kp_v = 0.015 , Kp_0 = 0.7;
#elif ENCODERINO == 3
float Kp_1 = 4 , Kff = 0.8 , Kd = 0.0, Kp_v = 0.015 , Kp_0 = 0.5;
#endif

float ref = 20;
float  error_ant, ref_ant;









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
float Kp,PID_p, PID_ff, PID_d;

void control_pos() {

  angleRead = getAngle();

#if ENCODERINO == 2
  error = ref - angleRead;
#elif ENCODERINO==3
  error = -ref + angleRead;
#endif

  t = millis();
  float dt = (t - t_ant) / 1000.0;
  float derror = error - error_ant;
  float dref = ref - ref_ant;

  if (fabs(error) < 0.4) PID = 0;
  else {
    if (fabs(error) < 1) Kp = Kp_0;
    else Kp = Kp_1;
    
    PID_p = Kp * error;
    PID_ff = Kff * dref / dt;
    PID_d = Kd * derror / dt;

    PID = PID_p + PID_ff + PID_d ;
  }

  // Kp=// Kp_v*fabs(error)+Kp_0;

  //calcula la velocidad

  error_ant = error;
  ref_ant = ref;
  t_ant = t;

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


float advanceSpline(float dt) {
  float t = (millis() - spline.getStart()) / 1000.0;
  float s = (spline.evaluate(t + dt) - spline.evaluate(t));
  float v = s / dt;

  long int nstep, n;
  n = 0;
  //una vuelta son 200 pasos y avanza 8 mm
  nstep = fabs((s / 8.0) * 200);
  unsigned long dly;
  // tengo que calcular la velocidad lineal ahora tengo velocidad angular

  dly = fabs((1.0 / (v / 60.0 * 200.0 ) / 2.0) * 1e6) / 9;

#ifdef ENCODERINO == 1
  nstep *= 8;
  dly /= 8;
#endif

  if (fabs(v) < 10e-3) {
    dly = 0;
    nstep = -1;
  }

  while (n <= nstep) {
    if (s == 0) continue;
    if (s > 0) {
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



  return spline.evaluate(t);
}



int id = 0;
int sentido;

#endif
