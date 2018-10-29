#include <SPI.h>
#include <Wire.h>

#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4 //Pin Dir del DRV8825
#define EN_PIN 6 //Pin enable del DRV8825

#define RESOLUTION 0.0219726 // 0.02197265625.... 360/(1<<14) Resolución del encoder 14 bit

#define ZM 0.5   // Zona muerta (Histéresis)
#define INIT 0 // Posición inicial
#define SLEEP 10   // Espera entre movimientos
#define STEP 100     // Ancho del pulso para el stepper

double ref = INIT;

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
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver

  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // Serial
  Serial1.begin(9600);            // Iniciar comunicación Serial al MEGA
  Serial1.setTimeout(10);         // Timeout de 10ms

  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización

  // Serial
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

void loop() {
  // Si llega algo por serial cambiar la referencia
  if ( Serial1.available() > 1) {           // Si se recibe algo por serial
    ref = (Serial1.parseFloat()); // Leer posición y mover husillo
    Serial1.parseFloat();               // Purgar
  }
  Serial.println(ref);

  // Calcular el error
  float err = dif(ref, leerEncoder());


  // Algoritmo todo o nada con histeresis
  // ZM zona muerta, SLEEP tiempo zona muerta.
  // STEP es la velocidad, si se hace variable se puede implementar un PID
  if (err > ZM) { // Si el error es positivo mover el motor hasta que sea 0


    do {
      digitalWrite(DIR_PIN, LOW);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(STEP);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(STEP);
      err = dif(ref, leerEncoder());
    } while (err > 0);


  } else if (err < (-ZM)) {  // Si el error es negativo mover el motor hasta que sea 0
    do {
      digitalWrite(DIR_PIN, HIGH);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(STEP);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(STEP);
      err = dif(ref, leerEncoder());
    } while (err < 0);


  }

  delay(SLEEP);

}
