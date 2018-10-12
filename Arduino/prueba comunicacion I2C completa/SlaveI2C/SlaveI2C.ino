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

#define SPEED_X 5000

double posX, theta1, theta2;
double ref = INIT;

void Initialize(){
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // I2C
  Wire.begin(0x11);                // Conectarse al bus I2C con la dirección indicada
  Wire.onReceive(receiveEvent); // Callback al recibir un paquete de datos

  // Serial
  Serial.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial.setTimeout(5);         // Timeout de 5ms

  // SPI
  SPISettings settings(10000000, MSBFIRST, SPI_MODE1);  // Parámetros de la comunicación (Datasheet AS5047D)
  SPI.begin();                                          // Iniciar la comunicación
  delay(1000);                                          // TODO (Probar si es necesario)
  SPI.beginTransaction(settings);                       // Empezar comunicación SPI

  digitalWrite(LED_BUILTIN, LOW); // Apagar LED

  digitalWrite(EN_PIN, LOW);  // Encender Driver

  
  
}

void sendStep(int delay_step, short int dir) {

  digitalWrite(DIR_PIN, ((dir >= 1) ? HIGH : LOW));
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(delay_step);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(delay_step);
}



float leerEncoder()  {
  long angulo;

  // COMUNICACIÓN SPI
  digitalWrite(CSN_PIN, LOW);
  byte msb = SPI.transfer(0xFF);
  byte lsb = SPI.transfer(0xFF);
  digitalWrite(CSN_PIN, HIGH);

  angulo = (((msb << 8) | lsb) & 0X3FFF); // msb | lsb (usando 14 bit)

  return ((float) angulo) * RESOLUTION; // 0.02197265625.... 360/(1<<14) Resolución del encoder 14 bit
}

// Diferencia entre ángulos (-180<phi<=180)
float dif(float a, float b) {
  double angulo = a - b;
  if (angulo < -180.0)
    return 360.0 + angulo;
  else if (angulo > 180.0)
    return -360.0 + angulo;
  return angulo;
}

void setup() {
  Initialize();
  }

// Tipo de dato para almacenar un float y poder acceder a sus bytes de forma cómoda
union Float {
  float raw;
  byte buffer[sizeof(float)];
};

/* -------------------------------------------------------------------------------- */

// Ahora si recibe el comando 0x01, lee un float detrás suya y actualiza la referencia

String Mensaje;
char mode; int ID_action; float param1; int param2;

void resetParam() {
  
  mode = '0';
  ID_action = 0;
  param1 = 0;
  param2 = 0;

}

void process_MSG(String mensaje) {
  
  String ID, p1, p2;
  ID = "";
  p1 = "";
  p2 = "";
  resetParam();

  //captura el modo
  mode = mensaje[0];
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


void receiveEvent(int howMany) {

  if (Wire.available() > 0) {
    Mensaje = "";
    for (int i = 0; i < howMany; i++)Mensaje += (char)Wire.read();
    process_MSG(Mensaje);
  }
}


/* -------------------------------------------------------------------------------- */


void move_X_to(long int positionX, float speedScrew) {

  long int nstep, n;
  float dly;
  float  distance = positionX - posX;

  nstep = 1;

  if (distance > 0) {
    posX += (double) nstep * 0.04;
  }
  else if (distance < 0) {
    posX -= (double) nstep * 0.04;
  }

  //una vuelta son 200 pasos y avanza 8 mm

  if (speedScrew == 0 || distance == 0) nstep = 0;

  // tengo que calcular la velocidad lineal ahora tengo velocidad angular
  //dly = fabs(1.0 /(speedScrew / 60.0 * 200.0 ) / 2.0)*1e6;

  dly = (1.0 / (speedScrew * 6e9));
  n = 0;

  while (n <= nstep) {
    if (distance > 0)sendStep(dly, 1);
    else sendStep(dly, 0);
    n++;
  }


}

void action(int ID_action, float param1, int param2) {
  switch (ID_action) {

    case 0: move_X_to(0, SPEED_X); break;
    case 1: break;
    case 2: break;
    case 3: move_X_to(param1, SPEED_X); break;
    case 4: break;

    case 10: break;
    case 11: break;
    case 12: break;
    case 14: break;

    case 20: break;

      //case 30: break;


  }

}

void loop() {

  Serial.print(ID_action);
  Serial.print(" ");
  Serial.print(param1);

  Serial.print(" posX= ");
  Serial.print(posX);
  Serial.println();

  // Si llega algo por serial cambiar la referencia
  if (ID_action == 30) {
    posX = 0;
    Serial.println("HOME");
  }
  else action(ID_action, param1, param2);


}
