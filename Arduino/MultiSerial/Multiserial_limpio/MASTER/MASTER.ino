#define DEBUG   // Descomentar para generar datos extra pos serial USB, INCOMPATIBLE CON MATLAB
#include "ENCODERINO.h"  // Clase Encoderino
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define ServoG A0
#define OLED_RESET 4
#define PWMA 8
#define AIN1 9
#define AIN2 10
#define BIN1 11
#define BIN2 12
#define PWMB 13

Adafruit_SSD1306 display(OLED_RESET);
Servo ServoGarra;

// Referencia a serial y pin del endstop
Encoderino encoder1(&Serial1, A1);
Encoderino encoder2(&Serial2, A2);
Encoderino encoder3(&Serial3, A3);

// Array para indexar
Encoderino * encoders[] = {&encoder1, &encoder2, &encoder3};

void setup() {
  // Comunicación USB
  Wire.begin();
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Iniciar encoderinos
  encoders[0]->init();
  encoders[1]->init();
  encoders[2]->init();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ServoGarra.attach(ServoG);
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
          ServoGarra.write(90);

#ifdef DEBUG                   // Debug muestra motor y orden
          Serial.print(motor);
          Serial.println(": 0");
#endif
          break;


        case 1:                          // Comando de posicion
          Serial.read();  Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Guarda el motor al que va destinada la orden
          q = Serial.parseFloat();       // Guarda la posición a la que debe ir el motor
          if (motor == 4) {
            ServoGarra.write(q);
          }
          else {
            encoders[motor - 1]->goPos(q); // Manda el motor correspondiente a la posición
          }

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

          encoders[motor - 1]->speed(q); // Manda el motor correspondiente a una velocidad

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


        case 4:                          // Comando de herramienta
          Serial.read();  Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Guarda el motor al que va destinada la orden
          q = Serial.parseFloat();       // Guarda la posición a la que debe ir el motor
          if (motor == 1) {              //Garra

          } else if (motor == 2) {        //Bomba
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, fabs(q));
          }

#ifdef DEBUG                   // Debug muestra motor y orden
          Serial.print(motor);
          Serial.print(": 4 ");
          Serial.println(q);
#endif
          break;


        case 20:                         // Comando para pedir posición
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Coger motor

          q = encoders[motor - 1]->getPos(); // Obtención de la posición

          char str[30];                  // Se manda la posición con formato %3.3f
          Serial.print("D1 M");
          Serial.print(motor);
          Serial.print(" ");
          Serial.println(dtostrf(q, 3, 3, str));
          break;

      }
    }
  }

  // Clear the buffer.
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  int x = encoders[0]->getPos();
  int y = 300 * (cos(encoders[1]->getPos() * M_PI / 180.0) + cos(encoders[2]->getPos() * M_PI / 180.0));
  int z = 300 * (sin(encoders[1]->getPos() * M_PI / 180.0) + sin(encoders[2]->getPos() * M_PI / 180.0));

  display.println("Q1: " + String((int) encoders[0]->getPos()) + "  X: " + String(x));
  display.println("Q2: " + String((int) encoders[1]->getPos()) + "  Y: " + String(y));
  display.println("Q3: " + String((int) encoders[2]->getPos()) + "  Z: " + String(z));

  display.display();
  delay(20);
}
