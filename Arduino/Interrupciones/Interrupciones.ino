boolean toggle2 = 0;
void setup() {
  pinMode(9, OUTPUT);
  noInterrupts();//stop interrupts
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  //Cambiar OCR2A para cambiar la frecuencia
  OCR2A = 249;// = (16*10^6) / (frequency*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  interrupts();//allow interrupts

}//end setup

ISR(TIMER2_COMPA_vect) { //timer1 interrupt toggles pin 9
  //generates pulse wave of frequency/2 (takes two cycles for full wave- toggle high then toggle low)
  if (toggle2) {
    digitalWrite(9, HIGH);
    toggle2 = 0;
  }
  else {
    digitalWrite(9, LOW);
    toggle2 = 1;
  }
}

void loop() {
}
