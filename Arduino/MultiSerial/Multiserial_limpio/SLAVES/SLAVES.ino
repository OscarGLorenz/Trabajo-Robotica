#define ENCODERINO 1  // COMPILACIÓN CONDICIONAL, 1,2 o 3. EN UN FUTURO A LA EEPROM
#define LOBOTOMIA LOW
#include <SPI.h>

#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4 //Pin Dir del DRV8825
#define EN_PIN 6 //Pin enable del DRV8825

#define ZM 1 //zona muerta

long int t = millis();
unsigned long dly = 0;
float angleRef, angleRead;
float error, Kp, PID;

float abs_angle;
float pastAngleRel = 0;
float HomeAngle = 256 ;
int nvueltas = 0;

float ref = 20;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  Serial1.begin(115200);         // Comunicación Serial a 115200 Baudios
  Serial1.setTimeout(10);    // Timeout de 5ms

  // SPI
  SPISettings settings(10000000, MSBFIRST, SPI_MODE1);  // Parámetros de la comunicación (Datasheet AS5047D)
  SPI.begin();                                          // Iniciar la comunicación
  delay(1000);                                          // TODO (Probar si es necesario)
  SPI.beginTransaction(settings);                       // Empezar comunicación SPI

  digitalWrite(LED_BUILTIN, LOW); // Apagar LED

  digitalWrite(EN_PIN, LOBOTOMIA);  // Encender Driver;
}
// Lee el encoder y devuelve en grados la posición
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
  if (abs(angulo) < ZM) return 0;
  else if (angulo < -180.0)
    return 360.0 + angulo;
  else if (angulo > 180.0)
    return -360.0 + angulo;
  return angulo;
}

// Posiciones en el endstop
#if  ENCODERINO == 1

#define HOME_ANGLE 0.0
#define AFTER_HOME 110.0
float offset = 0;

#elif  ENCODERINO == 2

#define HOME_ANGLE 100.0
#define AFTER_HOME 45.0;
float offset = 73+90;

#elif  ENCODERINO == 3
#define HOME_ANGLE -110.0
#define AFTER_HOME -45.0;
float offset = -120 + 30; //-100;

#endif

float getAngle() {

#if ENCODERINO == 1
  float angleRel = leerEncoder();
#elif ENCODERINO == 2
  float angleRel = 360.0 - leerEncoder();
#elif ENCODERINO == 3
  float angleRel = leerEncoder();
#endif

  if ((pastAngleRel >= 0 && pastAngleRel < 10) && (angleRel < 360 && angleRel > 350)) nvueltas += 1;
  else if ((pastAngleRel < 360 && pastAngleRel > 350) && (angleRel >= 0 && angleRel < 10)) nvueltas -= 1;

  pastAngleRel = angleRel;

  float fangle = HomeAngle - angleRel;
  return ((fangle + nvueltas * 360) / 5.0) + offset;
}


float speed = 0.0;// Velocidad objetivo

void control_pos() {

  Kp = 1.5;

  angleRead = getAngle();

#if ENCODERINO == 2
  error = ref - angleRead;
#elif ENCODERINO==3
  error = -ref + angleRead;

#endif

  //calcula la velocidad
  PID = Kp * error;

  dly = fabs(1.0 / (PID / 60.0 * 200.0 * 32.0 ) / 2.0) * 1e6;
  if (PID == 0) dly = 0;

  t = millis();
  while (millis() - t < 10) {

    if (dly == 0) continue;

    bool cond;
#if ENCODERINO == 1
    cond = (error <= 0);
#elif ENCODERINO == 2
    cond = (error >= 0);
#elif ENCODERINO == 3
    cond = (error >= 0);
#endif

    if (cond) {
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

bool homing = false;
bool homeDonete = false;

void advance(float distance, float speedScrew) {

  long int nstep, n;
  n = 0;
  //una vuelta son 200 pasos y avanza 8 mm
  nstep = fabs((distance / 8.0) * 200);

  unsigned long dly;
  // tengo que calcular la velocidad lineal ahora tengo velocidad angular
  dly = fabs((1.0 / (speedScrew / 60.0 * 200.0 ) / 2.0) * 1e6);

  #ifdef ENCODERINO == 1
    nstep *= 8;
    dly /= 8;
  #endif
  
  if (fabs(speedScrew) < 10e-3) {
    dly = 0; 
    nstep = -1;
  }

  while (n <= nstep) {
    // Si llega un ID=30 significa que se ha tocado el endstop
    // En el modo home se pasa a ir a after_home
    if (Serial1.available() > 0 &&  homing == true) {
      int id = Serial1.parseInt();
      Serial1.parseInt();

      if (id == 30) {
        homing = false;
        return;
      }
    }

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
int id = 0;
int sentido;

void loop() {
  // Si llega algo por el puerto serie
  if (Serial1.available()) {
    id = Serial1.parseInt();      // Guardar el ID

    float q;                          // Argumento auxiliar
    switch (id) {
      //case 0:// Ir al Home
      case 5:
        Serial1.parseInt();             // Purga el buffer

#if   ENCODERINO == 1
        homing = true;                // Rutina home
        advance(-1000, 150);           // Ir a un extremo
        advance(AFTER_HOME, 300);   // Ir a after home
        ref = AFTER_HOME;
        nvueltas = 0;
#elif ENCODERINO == 2
        homing = true;
        advance(100000, 10 * 32);
        nvueltas = -1;
        HomeAngle = getAngle();
        ref = AFTER_HOME;
#elif ENCODERINO == 3
        homing = true;
        advance(100000, 5 * 32);
        nvueltas = 0;
        HomeAngle = getAngle();
        ref = AFTER_HOME;
#endif
        homeDonete = true;
        break;


      case 1:
      // Ir a posición
        q = Serial1.parseFloat();    // Obtener posición
        Serial1.parseFloat();        // Purgar buffer


#if ENCODERINO == 1
        advance(q - ref, 120);     // Ir al la posición
        ref = q;                   // Guardar posición nueva
#else
        ref = q;
#endif

        break;

      case 2:                        // Ir a velocidad
        q = Serial1.parseFloat();    // Obtener posición
        Serial1.parseFloat();        // Purgar buffer

        speed = q;
        if (q > 0)  sentido = 1;
        else if (q < 0) sentido = -1;
        else sentido = 0;


        break;

      case 3:                        // Activar/Desactivar stepper
        Serial1.parseInt();          // Purgar Buffer

        // Toggle al pin del enable
        digitalWrite(EN_PIN, !digitalRead(EN_PIN));

        break;

    }
  } // ... end switch



#if ENCODERINO == 1
  if (fabs(speed) > 0.1 && id == 2) {
    advance(sentido * 1, fabs(speed) * 60.0 / 8.0);
    ref += sentido * 1;
  }

#elif ENCODERINO ==2
  if (homeDonete && id != 2)
    control_pos();
  else if (homeDonete) {
    advance((sentido * 1), fabs(speed)*5*32 / 6);
    getAngle();
  }


#elif ENCODERINO == 3
  if (homeDonete && id != 2)
    control_pos();
  else if (homeDonete && id == 2) {
    advance(-(sentido * 1),fabs( speed)*5*32 / 6);
    getAngle();
  }


#endif



  // Mandar cada cierto tiempo la referencia actual. En un futuro que sea la posición actual
  static long last = millis();
  if ((millis() - last) > 100) {
#if ENCODERINO == 1
    Serial1.println(ref + offset);
#else
    Serial1.println(getAngle());
#endif
    last = millis();
  }
}
