#include <Wire.h>

#define ENDSTOP_PIN A0
#define M1_address 0x11
#define M2_address 0x12
#define M3_address 0x13
#define STOP "J30"

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
  Serial.begin(115200); // Iniciar la comunicación Serial
  pinMode(ENDSTOP_PIN, INPUT);
}

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


void read_MSG() {

  //MSG="M1 S3 5";
  char auxc;
  if (Serial.available() > 3)MSG = "";
  while (Serial.available()) {
    auxc = (char)Serial.read();
    MSG += auxc;
  }
  process_MSG(MSG);
  Serial.print("MSG = ");
  Serial.println(MSG);
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
  m_address();
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

//Mandar por I2C
void I2C_Send (String code, int device) {
  Wire.beginTransmission(device);
  for (int i = 0; i < code.length(); i++)Wire.write(code[i]);
  delayMicroseconds(10);
  Wire.endTransmission();
}

//Realizar Request
void makeRequest(int device) {
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
    Dato[Nmotor]= aux_Dato.toFloat();
    Serial.print("Posicion motor ");
    Serial.println(Dato[Nmotor]);
    sig = '1';
  }
}

void setup() {
  initialize();
}
void loop() {
  read_MSG();
  if (analogRead(ENDSTOP_PIN) < 150 && (ID_action == 3 || ID_action == 0) && param1 <= 0) {
    I2C_Send(STOP, M1_address);
    delay(10);
    makeRequest(M1_address);
  }

  else {
    if (sig == '1') {
      I2C_Send(aux_MSG, address);
      sig = '0';
    }
    else if (sig == '0') {
      makeRequest(address);
      delay(10);
    }
  }
  Serial.println(sig);
}
