#include <Wire.h>

#define ENDSTOP_PIN1 A1
#define ENDSTOP_PIN2 A2
#define ENDSTOP_PIN3 A3
#define STOP "J30"
#define Q1 1
#define Q2 2
#define Q3 3
#define PC 0
#define T_OUT 10

//Variables globales
String MSG;
float param1;
char Type;
int ID_action;
char Nmotor;
char sig = '1';
int address;
String aux_MSG;
float Dato[3];
String aux_Dato;
String MSG_rec;

void initialize() {
  Wire.begin();         // Unirse al bus 12C
  Serial.begin(115200); // Iniciar la comunicación Serial
  Serial.setTimeout(T_OUT);
  Serial1.begin(115200);
  Serial1.setTimeout(T_OUT);
  Serial2.begin(115200);
  Serial2.setTimeout(T_OUT);
  Serial3.begin(115200);
  Serial3.setTimeout(T_OUT);
  pinMode(ENDSTOP_PIN1, INPUT);
}

void resetParam() {
  Type = '0';
  ID_action = 0;
  Nmotor = 0;
  ID_action = 0;
}

void read_MSG(int device) {

  //MSG="M1 S3 -5";
  char auxc;
  int flag = 0;
  switch (device) {
    case 0: {
        if (Serial.available() > 1) {
          MSG = Serial.readString();
          Serial.parseFloat();
          flag = 1;
        }

      } break;
    case 1: {
        if (Serial1.available() > 1) {
          MSG = Serial1.readString();
          flag = 1;
        }
      } break;
    case 2: {
        if (Serial2.available() > 1) {
          MSG = Serial2.readString();
          flag = 1;
        }
      } break;

    case 3: {

        if (Serial3.available() > 1) {
          MSG = Serial3.readString();
          flag = 1;
        }
      } break;

    default: break;
  }
  if (flag) {
    process_MSG(MSG);
    Serial_Send(aux_MSG, (int) (Nmotor-'0'));
    Serial.println(aux_MSG);
  }


}

void process_MSG(String mensaje) {

  int i = 0;
  String ID, p1;
  char nm;
  ID = "";
  p1 = "";
  resetParam();
  //Captura el motor
  mensaje.remove(0, 1);
  nm = mensaje[0];
  Nmotor = nm;

  mensaje.remove(0, 2);

  //Mensaje para mandar al esclavo
  aux_MSG = mensaje;

  //captura el modo
  Type = mensaje[0];
  mensaje.remove(0, 1);
  i = 0;

  //captura el ID
  while (mensaje[i] != ' ' && mensaje.length() >= i) {
    ID += mensaje[i];
    i++;
  }
  mensaje.remove(0, i + 1);
  ID_action = ID.toInt();
  i = 0;
  while (mensaje[i] != ' ' && mensaje.length() >= i) {
    p1 += mensaje[i];
    i++;
  }
  param1 = p1.toFloat();
}

//Mandar por Serial
void Serial_Send (String code, int device) {
  switch (device) {
    case 0: {
        Serial.println(code);
      } break;
    case 1: {
        Serial1.print(code);
         delay(10);
      } break;
    case 2: {
        Serial2.print(code);
        delay(10);
      } break;

    case 3: {
        Serial3.print(code);
         delay(10);
      } break;

    default: break;
  }
  delayMicroseconds(10);
}

//Realizar Request
void Serial_Read(int device) {

  if (Type == 'S') {
    Wire.requestFrom(device, 1);
    while (Wire.available()) {
      sig = (char) Wire.read();
    }
  }
  else if (Type == 'R') {
    Wire.requestFrom(device, sizeof(float));
    aux_Dato = "";
    while (Wire.available()) {
      //Dato[Nmotor] = Wire.parseFloat();
      aux_Dato += (char) Wire.read();
    }
    Dato[Nmotor] = aux_Dato.toFloat();
    Serial.print("Posicion motor ");
    Serial.println(Dato[Nmotor]);
    sig = '1';
  }
}

void setup() {
  initialize();
}
void loop() {

  if (analogRead(ENDSTOP_PIN1) < 150 ) {
    Serial1.print("-100");
    delay(50);
  }
  else if (analogRead(ENDSTOP_PIN2) < 150) {
    Serial2.print("-100");
    delay(50);
  }
  else if (analogRead(ENDSTOP_PIN3) < 150) {
    Serial3.print("100");
    delay(50);
  }
  else {
    read_MSG(PC);
    delay(50);
  }
}
