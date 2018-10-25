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
 
  Serial1.begin(9600);            // Iniciar comunicación Serial al MEGA
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

void loop() {
  if ( Serial1.available() > 1) {           // Si se recibe algo por serial
        advance(Serial1.parseFloat(), 300); // Leer posición y mover husillo
        Serial1.parseFloat();               // Purgar
  }
}
