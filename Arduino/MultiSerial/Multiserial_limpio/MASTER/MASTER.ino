#include "ENCODERINO.h"  // Clase Encoderino
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ServoG A0

#define OLED_RESET 4

// Pines del puente H
#define PWMA 8
#define AIN1 9
#define AIN2 10
#define BIN1 11
#define BIN2 12
#define PWMB 13

// Pines del encoder
#define ENCODER_1 2
#define ENCODER_2 3
#define ENCODER_SW 4

volatile long int encoder = 0;
void isr() {
  if (digitalRead(ENCODER_2)) {
    encoder--;
  } else {
    encoder++;
  }
}

Adafruit_SSD1306 display(OLED_RESET);

Servo ServoGarra;
unsigned long clawUntilMillis = 0;  // Tiempo funcionamiento de la garra

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

  attachInterrupt(digitalPinToInterrupt(ENCODER_1), isr, RISING); // Flanco bajada en el encoder
}

void loop() {

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

          break;


        case 2:                          // Comando de velocidad
          Serial.read(); Serial.read();  // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Guarda el motor al que va destinada la orden
          q = Serial.parseFloat();       // Guarda la posición a la que debe ir el motor

          encoders[motor - 1]->speed(q); // Manda el motor correspondiente a una velocidad

          break;


        case 3:                          // Comando de desactivar los stepper
          encoders[0]->disable();
          encoders[1]->disable();
          encoders[2]->disable();

          break;


        case 4:                          // Comando de herramienta
          Serial.read();  Serial.read(); // Eliminar " M" del buffer
          motor = Serial.parseInt();     // Guarda el motor al que va destinada la orden
          q = Serial.parseFloat();       // Guarda la posición a la que debe ir el motor
          if (motor == 1) { //Garra
            clawUntilMillis = fabs(q) + millis();
            if (q > 0) {
              analogWrite(PWMB,255);
              digitalWrite(BIN1,HIGH);
              digitalWrite(BIN2,LOW);
            } else {
              analogWrite(PWMB,255);
              digitalWrite(BIN1,LOW);
              digitalWrite(BIN2,HIGH);            
            }
          } else if (motor == 2) {        //Bomba
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, fabs(q));
          }

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



  /********GARRA********/
  if (clawUntilMillis < millis()) {
    analogWrite(PWMB,0);
    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2,LOW);  
  }
  /********GARRA********/


//  /********ENCODER********/
//  static bool read = 0;
//  read = digitalRead(ENCODER_SW);
//  static bool oldRead = 0;
//  static bool rising = false;
//  static long int timeLast;
//  bool NOguiado = false;
//  
//  if (read && !oldRead) {
//    timeLast = millis();
//    rising = true;
//  }
//
//  if ((millis() - timeLast) >= 80 && read && rising && millis() > 2000) {
//    NOguiado = !NOguiado;
//    rising = false;
//
//    encoders[1]->disable();
//    encoders[2]->disable();
//  }
//
//  oldRead = read;
//
//  if (!NOguiado && encoder != 0) {
//    static float pos = encoders[0]->getPos();
//    pos = encoders[0]->getPos() + encoder;
//    encoder = 0;
//    if (pos > 0 && pos < 180)
//      encoders[0]->goPos(pos);
//    Serial.println(pos);
//  }
//  /********ENCODER********/



  
  /********OLED********/
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  int x = encoders[0]->getPos();
  int y = 300 * (cos(encoders[1]->getPos() * M_PI / 180.0) + cos(encoders[2]->getPos() * M_PI / 180.0));
  int z = 300 * (sin(encoders[1]->getPos() * M_PI / 180.0) + sin(encoders[2]->getPos() * M_PI / 180.0));

  display.println("Q1: " + String((int) encoders[0]->getPos()) + "mm  X: " + String(x) + "mm");
  display.println("Q2: " + String((int) encoders[1]->getPos()) + "deg Y: " + String(y) + "mm");
  display.println("Q3: " + String((int) encoders[2]->getPos()) + "deg Z: " + String(z) + "mm");
  display.println("Q4: " + String((int) ServoGarra.read()) + "deg");

  display.display();
  /********OLED********/

}
