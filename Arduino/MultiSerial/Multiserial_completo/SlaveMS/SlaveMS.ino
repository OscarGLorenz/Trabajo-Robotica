#include "macros_var.h"
#define TIMEOUT 100
void setup() {
  Initialize();
  resetParam();
}

void loop() {

  Serial_receive();
  //printData();
  action(ID_action, param1, param2);
  if (ID_action == 2) control_loop_vel();
  else control_loop_pos();


}



/*********************************************************/
/********           ACCIONES Y REQUEST      **************/
/*********************************************************/

void action(int ID_action, float param1, int param2) {
  switch (ID_action) {

    case 0: theta1_ref = REF_HOME; break;
    case 1: break;
    case 2: Vref = param1; break;
    case 3: theta1_ref = param1; break;
    case 4: break;

    case 10: break;
    case 11: break;
    case 12: break;
    case 14: break;

    case 20: break;

    case 30: {
        theta1 = HOME;
        theta1_ref = HOME;
        break;
      }
    default: break;

  }
}

void requestEvent() {
  Serial.println("REQUEST EVENT");
  if (Type == 'R') {
    if (ID_action == 20) {
      aux_theta1 = String(theta1);
      for (int i = 0; aux_theta1.length() > i; i++)Wire.write(aux_theta1[i]);
      delay(1);
      done = '1';
    }
  }

  else if (Type == 'S') {
    switch (ID_action) {
      case 0: {
          //Caso HOME
          if (abs(theta1 - theta1_ref) <= DeadZone) done = '1';
          else done = '0';
          break;
        }
      case 2:
        { if (error_vel < 5)done = '1';
          else done = '0';
          break;
        }
      case 3: {
          if (abs(theta1 - theta1_ref) <= DeadZone) done = '1';
          else done = '0';
          break;
        }
      case 30: {
          done = '1'; break;
        }
      default: break;
    }
    Wire.write((char)done);
  }
  else Wire.write("ERROR");
}

/*********************************************************/
/********            BUCLES DE CONTROL      **************/
/*********************************************************/


void control_loop_pos() {
  t = millis();
  theta1 = leerEncoder();
  error_pos = difAngle(theta1_ref, theta1);

  PID_pos = Kp_pos * error_pos ;//+ Ki_pos * (error_pos - pastError_pos);
  dly = fabs(1 / (PID_pos)) * (150 / 32);
  pastError_pos = error_pos;
  
    if (dly != 0) {
      sendStep(dly, (PID_pos > 0) ? 1 : 0);
    }
  

}

void control_loop_vel() {

  t = millis();
  theta1 = leerEncoder();

  diff = (difAngle(theta1, angle0)) * TO_RAD;
  angle0 = theta1;

  inc_t = (millis() - t0) / 1000;

  V1 = diff / inc_t;

  t0 = millis();
  V1 = V0 + Kf_vel * (V1 - V0);
  V0 = V1;

  error_vel = Vref - V1;

  PID_vel = Vref + (Kp_vel * (1 + error_vel / 10)) * error_vel + Ki_vel * (error_vel - pastError_vel);
  pastError_vel = error_vel;

  //calcula la velocidad
  dly = fabs(1.0 / (PID_vel)) * (4.90873);

  while (millis() - t < 20) {
    if (dly != 0) {
      sendStep(dly, (PID_vel > 0) ? 1 : 0);
    }
  }
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

void Serial_receive() {
  if (Serial1.available() > 1) {
    MSG = "";
    Serial1.flush();
  }
  while (Serial1.available() > 1) {
    MSG += Serial1.readString();
  }
  //Serial.println(MSG);
  process_MSG(MSG);
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
  float aux = angulo;
  //if (fabs(angulo) < (DeadZone * 0.5)) return 0;
  if (angulo <= -180.0)
    aux = 360.0 + angulo;
  else if (angulo > 180.0)
    aux = -360.0 + angulo;
  if (aux < 0.5)aux = 0;
  return aux;
}

/*********************************************************/
/********           INICIALIZACION          **************/
/*********************************************************/


void resetParam() {
  Type = '0';
  ID_action = 0;
  param1 = 0;
  param2 = 0;

}


void Initialize() {
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // I2C

  // Serial
  Serial.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial.setTimeout(TIMEOUT);
  Serial1.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial1.setTimeout(TIMEOUT); // Timeout de 5ms

  // SPI
  SPISettings settings(10000000, MSBFIRST, SPI_MODE1);  // Parámetros de la comunicación (Datasheet AS5047D)
  SPI.begin();                                          // Iniciar la comunicación
  delay(10);                                          // TODO (Probar si es necesario)
  SPI.beginTransaction(settings);                       // Empezar comunicación SPI

  digitalWrite(LED_BUILTIN, LOW); // Apagar LED

  digitalWrite(EN_PIN, LOW);  // Encender Driver

}
