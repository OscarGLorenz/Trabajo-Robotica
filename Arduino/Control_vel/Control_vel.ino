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
  double aux;
  //zona muerta
  if (angulo < -180.0)
    aux=360.0 + angulo;
  else if (angulo > 180.0)
    aux =-360.0 + angulo;
  //if (aux<0.5) aux=0;
  return aux;
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
  Serial.setTimeout(5);         // Timeout de 5ms

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


float error,Kp,Kf,PID,t1;
float t0=0;
float V0 =0;
float V1=0;
float angle0=0;
float angle1=0;

float SpeedRef;
void loop() {
  
  /*if (Serial.available() > 0) Serial.readBytes(angleRef.buffer, sizeof(angleRef)); // Lee el angulo de referencia;
   
  angleRead.raw = leerEncoder();
  
  Serial.write(angleRead.buffer, sizeof(angleRead)); // Enviar a simulink la distancia captada
  */

  SpeedRef=-200;
  
  Kp=1;
  Kf=0.001;
  angleRead.raw = leerEncoder();
  t1=millis();  

  
  V1=difAngle(angleRead.raw,angle0)/(t1-t0);
//  if(fabs(V1-V0)>100)V1=V0;
  
  V1=V1*(50.0/3);
  Serial.print(V1);
  Serial.print("  ");
  V1=V0+Kf*(V1-V0);
  V0=V1;
  
  angle1=angle0;
  
  t0=t1;
  
  
  
  error=SpeedRef-V1;
  
  PID=SpeedRef+Kp*error;
  Serial.println(PID); 
  
  //calcula la velocidad
  
  dly = fabs(1.0/(PID))*(150/32)*1e3;
  
  if (PID == 0) dly = 0;

  t = millis();
  
  while (millis() - t < 20) {
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
