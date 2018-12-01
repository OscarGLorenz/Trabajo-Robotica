#define ENCODER_TIMEOUT 10
#define ENCODER_BAUDRATE 250000
#define DEBOUNCE_TIME_ENDSTOP 40


class Encoderino {
  public:
    // Se debe pasar una referencia al serial y el pin de su endstop
    Encoderino(HardwareSerial * s, uint8_t end_pin) {
      serial = s;
      endstop_pin = end_pin;
    }

    // Inicial el puerto serie
    void init(void) {
      serial->begin(ENCODER_BAUDRATE);
      //serial->setTimeout(ENCODER_TIMEOUT);
    }

    // ORDENES
    void goHome(void) {
      serial->println("8"); //Era un "0"
    }

    void goPos(float q) {
      char str[30];
      serial->print("1 "); serial->println(dtostrf(q, 3, 3, str));
    }

    void speed(float q) {
      char str[30];
      serial->print("2 "); serial->println(dtostrf(q, 3, 3, str));
    }

    void disable(void) {
      serial->println("3");
    }

    void write(String str) {
      serial->println(str);
    }

    // Endstop con anti-rebotes
    void update(void) {

      // Leer el pin del endstop
      int reading = digitalRead(endstop_pin);

      // Guardar la marca temporal del último flanco
      if (reading != lastButtonState)
        debounceTime = millis();

      // Si ha pasado tiempo suficiente desde el último flanco
      if ((millis() - debounceTime) > DEBOUNCE_TIME_ENDSTOP) {

        // Si hay un flanco
        if (reading != buttonState) {
          // Guardar nuevo estado
          buttonState = reading;

          // Si el flanco es de bajada
          if (buttonState == LOW) {
            serial->println("9");

            //delay(50);                               // DANGER DANGER DANGER DANGER OJO BLOQUEO PELIGROSO
          }
        }
      }

      // Guarda el estado anterior
      lastButtonState = reading;

      // Si llega información por el serial1 debe ser su posición
      if (serial->available() > 0) {
        char str[30];
        serial->readBytesUntil('\n',str,30);
        position = atof(str);
      }

    }

    // INFO
    float getPos(void) {
      return position;
    }

  private:
    HardwareSerial * serial;   // Referencia al puerto serie
    float position = 0;        // Posición actual del encoder en mm o deg
    uint8_t endstop_pin;       // Pin del endstop

    int lastButtonState = LOW; // Estado en el último ciclo del endstop
    long debounceTime = 0;     // Marca temporal del último flanco
    int buttonState = LOW;     // Estado del endstop
};
