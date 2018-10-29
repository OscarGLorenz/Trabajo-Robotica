#ifndef MACROS_VAR
#define MACROS_VAR

#include <SPI.h>
#include <Wire.h>

#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4 //Pin Dir del DRV8825
#define EN_PIN 6 //Pin enable del DRV8825

#define RESOLUTION 0.0219726 // 0.02197265625.... 360/(1<<14) Resolución del encoder 14 bit
#define DLY_FROM_RPM

#define ZM 0.4   // Zona muerta (Histéresis)
#define INIT 0 // Posición inicial
#define SLEEP 10   // Espera entre movimientos
#define STEP 100     // Ancho del pulso para el stepper

#define SPEED_THETA 20
#define REF_HOME 120
#define HOME 0.5
#define TO_RAD 0.01745329   //2*PI/360

/* DECLARACION DE VARIABLES GLOBALES*/

// Variables del mensaje

String MSG;
char Type;
int ID_action;
float param1;
int param2;

// variable de estado
char done = '1';
float theta1;  //theta1 o theta 2
float theta1_ref;  //theta1_ref, theta2_ref;
float Vref = SPEED_THETA;


// parametros
float K[3];
float  Amax, A2max;
float DeadZone = 1;


//PID THINGS

//pos

float Kp_pos=5;
float Ki_pos=0.01;
float error_pos=0;
float pastError_pos=0;
float PID_pos=0;

//vel

float Kp_vel=5;
float Ki_vel=0.0;
float Kf_vel=0.2;
float error_vel=0;
float pastError_vel=0;
float PID_vel=0;

float dly;
float t0 = 0;
float inc_t, t;
float V0 = 0;
float V1 = 0;
float angle0 = 0;
float angle1 = 0;


float diff;


String aux_theta1 = "";

// Tipo de dato para almacenar un float y poder acceder a sus bytes de forma cómoda
union Float {
  float raw;
  byte buffer[sizeof(float)];
};




#endif
