boolean toggle2 = 0;
volatile unsigned long int counter = 0;
long int timer=0;

void increase(){
  counter++;
}

void setup() {
  pinMode(9, OUTPUT);
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), increase, CHANGE);
  noInterrupts();                             //stop interrupts
  TCCR2A = 0;                                 // set entire TCCR2A register to 0
  TCCR2B = 0;                                 // same for TCCR2B
  TCNT2  = 0;                                 //initialize counter value to 0
  //Cambiar OCR2A para cambiar la frecuencia
  OCR2A = 20;                                // = (16*10^6) / (frequency(Hz)*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  interrupts();//allow interrupts
  Serial.begin(9600);
}
//end setup

ISR(TIMER2_COMPA_vect) {                      //timer1 interrupt toggles pin 9
  //generates pulse wave of frequency/2 (takes two cycles for full wave- toggle high then toggle low)
  if (toggle2) {
    DDRB|=B00000010;      //digitalWrite(9, HIGH);
    toggle2 = 0;
  }
  else {
    DDRB&=~B00000010;     //digitalWrite(9, LOW);
    toggle2 = 1;
  }
}

void loop() {

  if (millis() - timer >= 1000) {
    Serial.println(counter);
    timer = millis();
    counter = 0;
  }
}
