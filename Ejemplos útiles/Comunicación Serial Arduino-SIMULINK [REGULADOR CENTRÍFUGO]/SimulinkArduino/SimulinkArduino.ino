#include "MeOrion.h" //https://github.com/Makeblock-official/Makeblock-Libraries
#include <Wire.h>
#include <VL6180X.h> //https://github.com/pololu/vl6180x-arduino
#include <LCD.h>
#include <LiquidCrystal_I2C.h> //https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads

// Diferencia entre la x del modelo y la distancia medida con el sensor
#define OFFSET 12

// Velocidad máxima 
#define MAX_RPM 150

// Velocidad máxima 
#define GEAR_RATIO 0.689 //0.62.0/90.0

// Pines del encoder
#define ENCODER_1 2
#define ENCODER_2 4
#define ENCODER_SW 3

// Unión para convertir un float en su representación en bytes
union Float { 
  float f;
  byte buffer[sizeof(float)]; 
};

VL6180X sensor; // Sensor de distancia
MeEncoderNew motor(0x09, SLOT1); // Driver de Makeblock
LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7);  // Mapeo de pines del LCD

Float distance, speed, getSpeed; // Distancia, velocidad objetivo y velocidad actual

void setup() {
  // Inicialización
  distance.f = 0;
  speed.f = 0;
  getSpeed.f = 0;

  // Serial e I2C
  Serial.begin(9600);
  Serial.setTimeout(10);
  Wire.begin();
  
  // Configuración del LCD y Mensaje de bienvenida
  lcd.begin (16, 2);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
    
  // Configuración de Sensor de distancia
  delay(100);
  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(500);
  
}

long simulink = 0; // Variable para detectar si simulink esta comunicandose
void loop() {
  if (Serial.available() > 0) { // Si llega algo al buffer serial
    simulink = millis(); // Actualizar la variable simulink
    Serial.readBytes(speed.buffer, sizeof(speed)); // Leer la velocidad demandada
  }
    
  distance.f = sensor.readRangeSingle() + OFFSET; // Distancia del sensor, con el desfase correspondiente.

  motor.runSpeed((double) constrain(speed.f,0.0,MAX_RPM) * GEAR_RATIO); // Mover el motor limitando la velocidad máxima
  getSpeed.f =  motor.getCurrentSpeed()/GEAR_RATIO;
  
  Serial.write(distance.buffer, sizeof(distance)); // Enviar a simulink la distancia captada
  Serial.write(getSpeed.buffer, sizeof(getSpeed)); // Enviar a simulink la distancia captada

   // Si ha pasado un segundo sin que simulink envie nada, frenar el motor de forma progresiva
  if (millis() - simulink > 1000 ) {
    if (speed.f != 0) {
      lcd.home();
      lcd.print("    FRENANDO    ");
      while (speed.f >= 0) {
          motor.runSpeed((double) constrain(speed.f,0.0,MAX_RPM) * GEAR_RATIO);
          delay(200);
          speed.f -= 10;
      }
    }
      motor.runSpeed(speed.f = 0);
      lcd.home();
      lcd.print("      LISTO     ");
   } else if (simulink > 0) {
      lcd.home();
      lcd.print("    SIMULINK    ");
   }
  
  delay(10);
}





