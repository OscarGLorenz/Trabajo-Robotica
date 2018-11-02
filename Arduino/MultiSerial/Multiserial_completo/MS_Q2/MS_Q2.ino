// en este programa se trata de que el PID este integrado en el encoderino y que desde serial se le envie desde simulink una posicion de referencia

#include <SPI.h>

#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4 //Pin Dir del DRV8825
#define EN_PIN 6 //Pin enable del DRV8825

#define INIT 0 // Posición inicial
// Unión para convertir un float en su representación en bytes
union Float {
  float raw;
  byte buffer[sizeof(float)];
};

float leerEncoder()  {
  long angulo;

  // COMUNICACIÓN SPI
  digitalWrite(CSN_PIN, LOW);
  byte msb = SPI.transfer(0xFF);
  byte lsb = SPI.transfer(0xFF);
  digitalWrite(CSN_PIN, HIGH);

  angulo = (((msb << 8) | lsb) & 0X3FFF); // msb | lsb (usando 14 bit)

  return ((float) angulo) * 0.0219726;
}



// Diferencia entre ángulos (-180<phi<=180)
float difAngle(float a, float b) {
  double angulo = a - b;
  //zona muerta
  if (abs(angulo) < 1) return 0;
  else if (angulo < -180.0)
    return 360.0 + angulo;
  else if (angulo > 180.0)
    return -360.0 + angulo;
  return angulo;
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // Serial
  Serial.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial.setTimeout(10);
  Serial1.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial1.setTimeout(10);    // Timeout de 5ms

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

Float speed, angleRef, angleRead;


float error, Kp, PID;


float abs_angle;
int nvueltas = 0;
float pastAngleRel = 0;
float HomeAngle = 256;

float getAngle() {
  float angleRel = leerEncoder();

  if ((pastAngleRel >= 0 && pastAngleRel < 10) && (angleRel < 360 && angleRel > 350)) nvueltas += 1;
  else if ((pastAngleRel < 360 && pastAngleRel > 350) && (angleRel >= 0 && angleRel < 10)) nvueltas -= 1;

  pastAngleRel = angleRel;

  float fangle = HomeAngle - angleRel;
  return (fangle + nvueltas * 360) / 5.0;
}
float q1 = 20;

String MSG;
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
  int flag = 0;
  if (Serial1.available() > 1) {
    Serial1.flush();
    MSG = Serial1.readStringUntil('\n');
    Serial1.parseFloat();
    flag = 1;
  }
  if (flag) {
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


void control_pos() {



  Serial.print(getAngle());
  Serial.print(" ");
  Serial.println(q1);

  Kp =1.5;

  angleRead.raw = getAngle();

  Serial.println(angleRead.raw); // Enviar a simulink la distancia captada

  error = -q1 + angleRead.raw;
  //calcula la velocidad
  PID = Kp * error;

  dly = fabs(1.0 / (PID / 60.0 * 200.0 * 32.0 ) / 2.0) * 1e6;
  if (PID == 0) dly = 0;

  t = millis();
  while (millis() - t < 10) {
    if (dly == 0) continue;
    if (error >= 0) {
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
  }

}

void action(int ID, float p1, int p2) {
  switch (ID) {
    case 0: {
      } break;
    case 1: {
      } break;
    case 2: {
        q1 = p1;
      } break;
    case 3: {
      } break;

    case 20: {
      } break;
    case 30: {
        q1 = 0;
        nvueltas = 0;
        HomeAngle = leerEncoder();
      } break;
    default:;

  }

}

void loop() {
  q1=50 ;
  control_pos();
  /*read_serial1();
  action(ID_action, param1, param2);
  if (ID_action != 3 && ID_action != 30) {
    control_pos();*/
  
 // else 
 Serial1.println(angleRead.raw);
}




//}
