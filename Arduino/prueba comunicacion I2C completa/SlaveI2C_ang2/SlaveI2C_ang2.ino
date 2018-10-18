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
#define REF_HOME -50


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

// parametros
float K[3];
float  Amax, A2max;
float Vmax = SPEED_THETA;
float DeadZone = 0.04 ;

//PID THINGS
float Kp, Ki, Kf;

float error, pastError, PID, dly;
float t0 = 0;
float inc_t, t;
float Past_error = 0;
float V0 = 0;
float V1 = 0;
float angle0 = 0;
float angle1 = 0;
float SpeedRef = 10;
float diff;


// Tipo de dato para almacenar un float y poder acceder a sus bytes de forma cómoda
union Float {
  float raw;
  byte buffer[sizeof(float)];
};



void setup() {
  Initialize();
}



/* -------------------------------------------------------------------------------- */

// Ahora si recibe el comando 0x01, lee un float detrás suya y actualiza la referencia

void resetParam() {

  Type = '0';
  ID_action = 0;
  param1 = 0;
  param2 = 0;

}



String aux_theta1 = "";





void requestEvent() {
  if (Type == 'R') {
    if (ID_action == 20) {
      aux_theta1 = String(theta1);
      for (int i = 0; aux_theta1.length() > i; i++)Wire.write(aux_theta1[i]);
      delay(10);
      done = '1';
    }
  }
  else {
    switch (ID_action) {
      case 0: {
          //Caso HOME
          if (abs(theta1 - theta1_ref) <= DeadZone) done = '1';
          else done = '0';
          break;
        }
      case 2:
        { if (error < 10)done = '1';
          else done = '0';
          break;
        }
      case 3: {
          if (abs(theta1 - theta1_ref) <= DeadZone) done = '1';
          //else done = '0';
          break;
        }
    }
  }

  Wire.write(done);
  delay(10);

}

/* -------------------------------------------------------------------------------- */


void action(int ID_action, float param1, int param2) {
  switch (ID_action) {

    case 0: theta1_ref = REF_HOME; break;
    case 1: break;
    case 2: {
        Vmax = param1;
        //control_loop_vel();
        break;
      }
    case 3: theta1_ref = param1; break;
    case 4: break;

    case 10: break;
    case 11: break;
    case 12: break;
    case 14: break;

    case 20: break;

      //case 30: break;

  }
}


// antes de cada vuelta del loop se realizan los callbacks del wire.

void loop() {
  action(ID_action, param1, param2);
  if (ID_action != 2) control_loop_pos;
}


/*********************************************************/
/********            BUCLES DE CONTROL      **************/
/*********************************************************/


void control_loop_pos() {
  Kp = 2;
  Ki = 0.1;
  t = millis();
  while (millis() - t < 10) {
    theta1 = leerEncoder();
    error = difAngle(theta1_ref, theta1);
    PID = Kp * error + Ki * (error - pastError);
    dly = fabs(1 / (PID)) * (150 / 32);
    pastError = error;
    if (dly != 0) {
      sendStep(dly, (PID > 0) ? 1 : 0);
    }
  }
}

void control_loop_vel() {

  Kp = 5;
  Ki = 0.5;
  Kf = 0.2;
  SpeedRef = 1;
  t = millis();


  theta1 = leerEncoder();

  diff = (difAngle(theta1, angle0)) * TO_RAD;
  angle0 = theta1;

  inc_t = (millis() - t0) / 1000;

  V1 = diff / inc_t;

  t0 = millis();
  V1 = V0 + Kf * (V1 - V0);
  V0 = V1;

  error = SpeedRef - V1;

  PID = SpeedRef + (Kp * (1 + error / 10)) * error + Ki * (error - Past_error);
  Past_error = error;

  //calcula la velocidad

  dly = fabs(1.0 / (PID)) * (4.90873);

  while (millis() - t < 2000) {
    if (dly != 0) {
      sendStep(dly, (PID > 0) ? 1 : 0);
    }
  }
  Serial.println(V1);
}

/*********************************************************/
/********            IMPRIMIR DATOS         **************/
/*********************************************************/

void printData() {
  Serial.print("ID= ");
  Serial.print(ID_action);
  Serial.print(" param1= ");
  Serial.print(param1);
  Serial.print(" theta1= ");
  Serial.print(theta1);
  Serial.print(" theta1_ref = ");
  Serial.print(theta1_ref);
  Serial.print(" done= ");
  Serial.print(done);
  Serial.print(" V1= ");
  Serial.print(V1);
  Serial.println();
}


/*********************************************************/
/********           GESTION DEL I2C         **************/
/*********************************************************/

void receiveEvent(int howMany) {
  if (Wire.available() > 0) {
    MSG = "";
    for (int i = 0; i < howMany; i++)MSG += (char)Wire.read();
    process_MSG(MSG);
  }
}


/*********************************************************/
/********           GESTION DEL MENSAJE     **************/
/*********************************************************/

void process_MSG(String mensaje) {

  String ID, p1, p2;
  ID = "";
  p1 = "";
  p2 = "";
  resetParam();

  //captura el modo
  Type = mensaje[0];
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


/*********************************************************/
/********                STEPPER            **************/
/*********************************************************/

void sendStep(int delay_step, short int dir) {
  digitalWrite(DIR_PIN, ((dir >= 1) ? HIGH : LOW));
  digitalWrite(STEP_PIN, HIGH);
  delay(delay_step);
  digitalWrite(STEP_PIN, LOW);
  delay(delay_step);
}

/*********************************************************/
/********               ENCODER             **************/
/*********************************************************/

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

float difAngle(float a, float b) {

  float angulo = a - b;
  //if (fabs(angulo) < (DeadZone * 0.5)) return 0;
  if (angulo < -180.0)
    return 360.0 + angulo;
  else if (angulo > 180.0)
    return -360.0 + angulo;
  else return angulo;
}
/*********************************************************/
/********           INICIALIZACION          **************/
/*********************************************************/


void Initialize() {
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // I2C
  Wire.begin(0x11);                // Conectarse al bus I2C con la dirección indicada
  // Callback al recibir un paquete de datos
  Wire.onReceive(receiveEvent);
  // Callback del request
  Wire.onRequest(requestEvent);

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
