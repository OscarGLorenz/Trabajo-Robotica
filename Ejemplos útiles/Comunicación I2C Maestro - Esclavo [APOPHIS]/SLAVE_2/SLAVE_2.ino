#include <TinyWireS.h> // I2C para ATTiny84
#include <SoftwareServo.h> // Servos para ATTiny84

// PINOUT
#define SERVO 9
#define ADDRESS (byte []) {8, 1, 2, 3, 10}

// Máximo pulso y mínimo pulso
#define MIN_MS 800
#define MAX_MS 2200

byte address;

// Objeto de servo
SoftwareServo servo;
bool detached = false;

void setup() {
  for (int i = 0; i < 5; i++)
    address |= digitalRead(ADDRESS[4 - i]) ? 0 : _BV(i);

  //Inicia el Bus I2C con la dirección obtenida
  TinyWireS.begin(8);

  // Configuración del servo
  servo.attach(SERVO);
  servo.setMinimumPulse(MIN_MS);
  servo.setMaximumPulse(MAX_MS);

}

void loop() {
  // Si hay bytes en el bus I2C
  if (TinyWireS.available()) {
    byte angle = TinyWireS.receive();
    if (angle == 255) {
      servo.detach();
      detached = true;
    } else if (detached) {
      detached = false;
      servo.attach(SERVO);
      servo.write(angle);
    } else {
      // Mover el servo a posición que indica el byte recibido
      servo.write(angle);
    }
  }

  // Actualiza la posición del servo
  SoftwareServo::refresh();
}

