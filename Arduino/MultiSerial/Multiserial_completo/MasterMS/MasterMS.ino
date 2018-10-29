#include <Wire.h>

#define ENDSTOP_PIN A0
#define STOP "J30"
#define Q1 2
#define Q2 1
#define Q3 3
#define PC 0

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


void initialize() {
  Wire.begin();         // Unirse al bus 12C
  Serial.begin(9600); // Iniciar la comunicación Serial
  Serial.setTimeout(10);
  Serial1.begin(9600);
  Serial1.setTimeout(10);
  Serial2.begin(9600);
  Serial2.setTimeout(10);
  Serial3.begin(9600);
  Serial3.setTimeout(10);
  pinMode(ENDSTOP_PIN, INPUT);
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

  switch (device) {
    case 0: {
        if (Serial.available() > 1) {
          MSG = Serial.readString();
        }
      } break;
    case 1: {
        if (Serial1.available() > 1) {
          MSG = Serial1.readString();
        }
      } break;
    case 2: {

        if (Serial2.available() > 1) {
          MSG = Serial2.readString();
        }
      } break;

    case 3: {

        if (Serial3.available() > 1) {
          MSG = Serial3.readString();
        }
      } break;

    default: break;
  }


}

void process_MSG(String mensaje) {

  int i = 0;
  String ID, p1;
  ID = "";
  p1 = "";
  resetParam();

  //Captura el motor
  mensaje.remove(0, 1);
  Nmotor = mensaje[0];

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
      } break;
    case 2: {
        Serial2.print(code);
        delay(10);
        while (Serial2.available () < (MSG.length()+2) && Serial2.read()=='e') {
          delay(10);
        }
      } break;

    case 3: {
        Serial3.print(code);
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

  read_MSG(PC);

  /*if (analogRead(ENDSTOP_PIN) < 150 && (ID_action == 3 || ID_action == 0) && param1 <= 0) {
    I2C_Send(STOP, M1_address);
    delay(10);
    Serial.print("STOP");
    makeRequest(M1_address);
    }*/

  if (analogRead(ENDSTOP_PIN) < 150 ) {

  }

  Serial_Send(MSG, Q2);

  /*else {
    if (sig == '1') {
      I2C_Send(aux_MSG, address);
      sig = '0';
    }
    else if (sig == '0') {
      makeRequest(address);
      delay(10);
    }
    }*/

  Serial.println(MSG);


}
