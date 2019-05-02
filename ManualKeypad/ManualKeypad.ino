/*El objetivo de este programa es controlar manualmente (sin librerias) un teclado matricial
 * 
 */
#define M_FILA  PORTB
#define M_ROW   PORTA

volatile int mF=0,mR=0;
volatile int lastF,lastR;
void gKey(int fila, int columna);

void setup() {
  DDRB=255;                            //(255=0b1111 1111)
  Serial.begin(9600);
  cli();                              //deshabilito temporalmente las interrupciones
  TCCR1A=0;                           //limpiamos los registros de control
  TCCR1B=0;                           //del timer 1

  OCR1A=156;
  TCCR1B|=(1<<WGM12);
  TCCR1B|=(1<<CS10);
  TCCR1B|=(1<<CS12);
  
  
  TIMSK1|=(1<<OCIE1A);                  //enciendo el timer1
  
  sei();                              //activamos las interrupciones
  M_FILA=0;
  M_ROW=7;
}

void loop() {
  int key=PINA;
  if(key!=7)gKey(PINB,PINA);
}

ISR(TIMER1_COMPA_vect){                 //ISR es la rutina de interrupcion
  mF++;
  if(mF<=3){
    M_FILA=mF;
  }
  else{
    mF=0;
    if(mR<3){
      mR++;
    }
    else
      mR=0;
  }
}

void gKey(int fila,int columna){
//  TIMSK1&=~(1<<OCIE1A);
  while(lastF==fila&&lastR==columna)gKey(PINB,PINA);
  lastF=fila;
  lastR=columna;
  Serial.print(columna);
  Serial.print(";");
  Serial.println(fila);
//  TIMSK1|=(1<<OCIE1A);                  
}
