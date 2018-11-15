#ifndef ENCODER_H_
#define ENCODER_H_
#include <SPI.h>
#define CSN_PIN 11 //Pin slave-select del AS5047D
#define STEP_PIN 5 //Pin Step del DRV8825
#define DIR_PIN 4 //Pin Dir del DRV8825
#define EN_PIN 6 //Pin enable del DRV8825

class Encoder {
public:
void init(void) {
  pinMode(LED_BUILTIN, OUTPUT); // LED
  pinMode(CSN_PIN, OUTPUT);     // Chip Select SPI
  pinMode(STEP_PIN, OUTPUT);    // Step Driver
  pinMode(DIR_PIN, OUTPUT);     // Dir Driver
  pinMode(EN_PIN, OUTPUT);      // Enable Driver

  digitalWrite(LED_BUILTIN, HIGH); // Encender LED, inicialización
  
  // Parámetros de la comunicación (Datasheet AS5047D)
  SPISettings settings(10000000, MSBFIRST, SPI_MODE1);
  SPI.begin();                     // Iniciar la comunicación
  delay(1000);                     // TODO (Probar si es necesario)
  SPI.beginTransaction(settings);  // Empezar comunicación SPI

  digitalWrite(LED_BUILTIN, LOW); // Apagar LED

}

void enable(bool en) {
  digitalWrite(EN_PIN, en);  // Encender Driver;
}
void togglEnable() {
   digitalWrite(EN_PIN, !digitalRead(EN_PIN));
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
} encoder;

#endif

