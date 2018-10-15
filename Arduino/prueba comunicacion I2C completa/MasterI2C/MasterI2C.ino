#include <Wire.h>

#define ENDSTOP_PIN A0
#define M1_address 0x11
#define M2_address 0x12
#define M3_address 0x13
#define STOP  "J30"

void initialize() {
  Wire.begin();         // Unirse al bus 12C
  Serial.begin(115200); // Iniciar la comunicación Serial
  pinMode(ENDSTOP_PIN, INPUT);
}

int sig = 1;
char Nmotor;
char Type;
int ID_action;
int address;
String aux_MSG;
float Dato[3];

void resetParam() {
  Type = '0';
  ID_action = 0;
  Nmotor = 0;
  ID_action = 0;
}

void m_address() {
  if (Nmotor == '1') {
    address = M1_address;
  }
  else if (Nmotor == '2') {
    address = M2_address;
  }
  else if (Nmotor == '3') {
    address = M3_address;
  }
}

void process_MSG(String mensaje) {

  int i = 0;
  String ID;
  ID = "";
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

  ID_action = ID.toInt();
}

void setup() {
  initialize();
}

void I2C_Send (String code, int device) {
  Wire.beginTransmission(device);
  for (int i = 0; i < code.length(); i++)Wire.write(code[i]);
  delayMicroseconds(10);
  Wire.endTransmission();
}

//Realizar la request

void makeRequest(int device) {
  if (Type == 'S') {
    Wire.requestFrom(device, 1);
    while (Wire.available()) {
      sig = (int) Wire.read();
    }
  }
  else if (Type == 'R') {
    Wire.requestFrom(device, sizeof(float));
    while (Wire.available()) {
      Dato[nmotor] = Wire.parseFloat();
    }
    sig = 1;
  }
}
void loop() {
  if (analogRead(ENDSTOP_PIN) < 150) {
    I2C_Send(STOP, M1_address);
    delay(10);
    makeRequest(M1_address);
  }
  else {
    if (Type == 'S') {
      I2C_Send(aux_MSG, address);
    }
    else {
      makeRequest(address);
      delay(10);
    }
  }
  Serial.println(sig);
}
