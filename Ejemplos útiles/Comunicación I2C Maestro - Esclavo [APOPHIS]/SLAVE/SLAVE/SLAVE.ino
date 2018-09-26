#include <TinyWireS.h> // I2C para ATTiny84
#include <SoftwareServo.h> // Servos para ATTiny84

// PINOUT
#define AD0 8
#define AD1 1
#define AD2 2
#define AD3 3
#define AD4 10
#define SERVO 9
//#define ADDRESS (byte []) {8, 1, 2, 3, 10}

// Máximo pulso y mínimo pulso
#define MIN_MS 800
#define MAX_MS 2200

// Union de un campo de 8 bits y un byte para no usar máscaras
union Address {
  struct {
    unsigned b0: 1, b1: 1, b2: 1, b3: 1, b4: 1, b5: 1, b6: 1, b7: 1;
  } Bit;
  byte Byte = 0;
} address;

// Objeto de servo
SoftwareServo servo;

//  address |= digitalRead(ADDRESS[i]) ? 0 : _BV(i);  

void setup() {
  // Lectura de los pines de dirección
  address.Bit.b0 = digitalRead(AD0);
  address.Bit.b1 = digitalRead(AD1);
  address.Bit.b2 = digitalRead(AD2);
  address.Bit.b3 = digitalRead(AD3);
  address.Bit.b4 = digitalRead(AD4);
  
  //Inicia el Bus I2C con la dirección obtenida
  TinyWireS.begin(address.Byte);

  // Configuración del servo
  servo.attach(SERVO);
  servo.setMinimumPulse(MIN_MS);
  servo.setMaximumPulse(MAX_MS);

}

void loop() {
  // Si hay bytes en el bus I2C
  if (TinyWireS.available())
    // Mover el servo a posición que indica el byte recibido
    servo.write(TinyWireS.receive());

  // Actualiza la posición del servo
  SoftwareServo::refresh();
}
