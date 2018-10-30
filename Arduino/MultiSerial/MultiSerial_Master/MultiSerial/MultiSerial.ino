#define ENDSTOP_PIN A0
void setup() {
  Serial.begin(9600);  Serial.setTimeout(10); // Serial USB del MEGA

  Serial1.begin(9600); Serial1.setTimeout(10); // Serial 1 del MEGA
  Serial2.begin(9600); Serial2.setTimeout(10); // Serial 2 del MEGA
  Serial3.begin(9600); Serial3.setTimeout(10); // Serial 3 del MEGA

}
void loop() {
  if (analogRead(ENDSTOP_PIN) > 150) {
    if (Serial.available() > 1) {           // Si se lee algo por el serial USB
      int motor = Serial.parseInt();      // Que motor se elige
      float q = Serial.parseFloat();      // Posición del motor
      Serial.print(motor);
      Serial.print(" ");
      Serial.println(q);
      switch (motor) {
        case 1:
          Serial1.println(q);               // Mover motor 1
          break;

        case 2:
          Serial2.println(q);               // Mover motor 2
          break;

        case 3:
          Serial3.println(q);               // Mover motor 3
          break;
      }
    }
  }
  else {
    Serial2.println(-100);
  }
}
