// este programa sirve para controlar el avance del husillo de paso 8mm (200 pasos son 1 vuelta)
#include <SPI.h>

#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4  //Pin Dir del DRV8825
#define EN_PIN 6   //Pin enable del DRV8825

#define INIT 0 // Posición inicial

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);   // LED
  pinMode(CSN_PIN, OUTPUT);       // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);      // Step Driver
  pinMode(DIR_PIN, OUTPUT);       // Dir Driver
  pinMode(EN_PIN, OUTPUT);        // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH);// Encender LED, inicialización

  Serial1.begin(115200);            // Iniciar comunicación Serial al MEGA
  Serial1.setTimeout(10);         // Timeout de 10ms

  // SPI
  SPISettings settings(10000000, MSBFIRST, SPI_MODE1);  // Parámetros de la comunicación (Datasheet AS5047D)
  SPI.begin();                                          // Iniciar la comunicación
  delay(1000);                                          // TODO (Probar si es necesario)
  SPI.beginTransaction(settings);                       // Empezar comunicación SPI

  digitalWrite(LED_BUILTIN, LOW); // Apagar LED

  digitalWrite(EN_PIN, LOW);  // Encender Driver
}

long int t = millis();
unsigned long dly = 0;

// Función para avanzar "distance" milimetros y a una velocidad "speedScrew"
void advance(float distance, float speedScrew) {

  long int nstep, n;
  n = 0;
  //una vuelta son 200 pasos y avanza 8 mm
  nstep = abs((distance / 8.0) * 200);

  if (speedScrew == 0) dly = 0;

  // tengo que calcular la velocidad lineal ahora tengo velocidad angular
  dly = fabs((1.0 / (speedScrew / 60.0 * 200.0 ) / 2.0) * 1e6);

  while (n <= nstep) {

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


float x = 0;
float xref = 0;

String MSG="";
int ID_action;
float param1;
int param2;

void resetParam() {
//  Type = '0';
  ID_action = 0;
  param1 = 0;
  param2 = 0;

}


void read_serial1() {
  int flag=0;
  if (Serial1.available() > 1) {
    Serial1.flush();
    MSG = Serial1.readStringUntil('\n');
    Serial1.parseFloat();
    flag=1;
  }
  if (flag==1) {
   process_MSG(MSG);
   Serial.println(MSG);
  }
}

void process_MSG(String mensaje) {

  String ID, p1, p2;
  ID = "";
  p1 = "";
  p2 = "";
  resetParam();

  //captura el modo
//  Type = mensaje[0];
  mensaje.remove(0, 1);
  int i = 0;

  //captura el ID
  while (mensaje[i] != ' ' && mensaje.length() >= i) {
    ID += mensaje[i];
    i++;
  }

  mensaje.remove(0, i + 1);
  ID_action = ID.toInt();


  if (ID_action == 5); //ir a home

  //si hay que capturar parámetros

  else {
    //captura el 1er parametro
    i = 0;
    while (mensaje[i] != ' ' && mensaje.length() >= i) {
      p1 += mensaje[i];
      i++;
    }
    mensaje.remove(0, i + 1);


    if (ID_action == 4) {
      param1 = p1.toInt();

      //captura el parametro 2
      //captura el 1er parametro
      i = 0;
      while (mensaje[i] != ' ' && mensaje.length() >= i) {
        p2 += mensaje[i];
        i++;
      }
      mensaje.remove(0, i);

      param2 = p2.toInt();
    }
    else {
      param1 = p1.toFloat();
    }

  }

}



void loop() {

  read_serial1();
  xref=param1;
  Serial.println(xref);

  if (xref < -90) {
    xref =3;
    x = 0;
  }

  if ((xref - x) > 2) {
    advance(0.1, 300); // Leer posición y mover husillo
    x+=0.1;
  }
  else if ((xref - x) < -2) {
    advance(-.1, 300); // Leer posición y mover husillo
    x=-0.1;
  }

}
