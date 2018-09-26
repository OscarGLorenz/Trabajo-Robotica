#define SAMPLES 320      // Muestras por cada lectura
#define BAUD_RATE 57600  // Baudios de la conexión serial

#define B_UIN B00001000  // Pin D4 en binario, el puerto es el D
#define USENSE A0        // Tensión a la entrada del RLC Serie
#define ISENSE A1        // Corriente en el RLC a razón de 1mA/V
#define UOUT A2          // Tensión a la salida del RLC (Condensador)


/***** ALMACENAMIENTO DE LAS LECTURAS DEL ADC *****/
#define DATASIZE 10                                     // El ADC tiene una resolución de 10 bits
#define UINTSIZE 16                                     // Tamaño de un entero sin signo
#define MAXBINUINT (unsigned int)(pow(2, UINTSIZE) - 1) // Mayor valor representable por un entero de 16 bit
#define REALMAX (5 * DATASIZE * UINTSIZE)               // Número de valores de 16 bit que almacenaremos
#define VIRTUALMAX (5 * UINTSIZE * UINTSIZE)            // Número de valores de 10 bit que almacenaremos

const unsigned int dataMask = ~(MAXBINUINT << DATASIZE);// Máscara auxiliar
const unsigned int savedBits = UINTSIZE - DATASIZE;     // Bits ahorrados por cada palabra de 2 bit
const float ratio = (float)UINTSIZE / (float)DATASIZE;  // Ratio de bits ahorrados
unsigned int virtualArray[REALMAX];                     // Array donde guardaremos los valores de 10 bit

// Leer un entero de 10 bit del array dando el índice
unsigned int readvirtualArray(unsigned int virtualIndex) {
  unsigned int realIndex = floor(virtualIndex / ratio); // Índice real en el array
  unsigned int firstBit = (DATASIZE * virtualIndex) % UINTSIZE; // Primer bit de la palabra
  unsigned int firstBitComp = UINTSIZE - firstBit;     
  unsigned int value = (virtualArray[realIndex] & (dataMask << firstBit)) >> firstBit;
  if (firstBit > savedBits) {  // value spans 2 int's
    value = value | ((virtualArray[realIndex + 1] & (dataMask >> firstBitComp)) << firstBitComp);
  }
  return value;
}

// Guardar un valor de 10 bit en el array en el índice indicado
void writevirtualArray(unsigned int virtualIndex, unsigned int value) {
  unsigned int realIndex = floor(virtualIndex / ratio); // Índice real en el array
  unsigned int firstBit = (DATASIZE * virtualIndex) % UINTSIZE; // Primer bit de la palabra
  unsigned int firstBitComp = UINTSIZE - firstBit;
  virtualArray[realIndex] = (virtualArray[realIndex] & ~(dataMask << firstBit)) | (value << firstBit);
  if (firstBit > savedBits) {  // value spans 2 int's
    virtualArray[realIndex + 1] = (virtualArray[realIndex + 1] & ~(dataMask >> firstBitComp)) | (value >> firstBitComp);
  }
}
/***** ALMACENAMIENTO DE LAS LECTURAS DEL ADC (Corriente y Tensión a la salida)*****/



/***** ALMACENAMIENTO DE LAS LECTURAS DEL ADC (Tensión a la entrada, de forma binaria) *****/
byte booldata[40]; // 40*8 valores (320)

// Guardar un bit en el array en el índice indicado
void writeBoolArray(unsigned int virtualIndex, bool value) {
  unsigned int realIndex = virtualIndex / 8;
  byte maskPos = virtualIndex % 8;
  booldata[realIndex] |= (byte) value << maskPos;
}

// Leer un bit del array en el índice indicado
bool readBoolArray(unsigned int virtualIndex) {
  unsigned int realIndex = virtualIndex / 8;
  byte maskPos = virtualIndex % 8;
  return booldata[realIndex] & ((byte) 1 << maskPos);
}
/***** ALMACENAMIENTO DE LAS LECTURAS DEL ADC (Tensión a la entrada, de forma binaria) *****/



// Macros auxiliares para manipulación del registro del ADC
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup() {
  // Registro ADC, acelerar lecturas
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;

  DDRD |= B_UIN;           // pinMode(4,OUTPUT)

  Serial.begin(BAUD_RATE); // Iniciar comunicación serial
}

unsigned long wait = 0;
unsigned short i = 0;

void loop() {
  PORTD &= ~B_UIN;                      // digitalWrite(4,0)
  while (i < SAMPLES) {                 // Muestrear
    if (i == SAMPLES/2) PORTD |= B_UIN; // digitalWrite(4,1)
    if (micros() - wait >= 500) {       // Tomar una muestra cada 500 us
      wait = micros();
      writevirtualArray(i, analogRead(UOUT));             // Leer y guardar tensión de salida
      writevirtualArray(i + SAMPLES, analogRead(ISENSE)); // Leer y guardar corriente
      writeBoolArray(i, analogRead(USENSE) >= 512);       // Leer y guardar tensión de entrada
      i++;
    }
  }
  i = 0;
  for (int j = 0; j < SAMPLES; j++) {                 // Mandar las muestras
    Serial.print(readvirtualArray(j));                // Mandar la tensión de salida
    Serial.print(" ");
    Serial.print(readvirtualArray(j + SAMPLES));      // Mandar la corriente
    Serial.print(" ");
    Serial.println((unsigned int) readBoolArray(j));  // Mandar la tensión de entrada
  }
}

