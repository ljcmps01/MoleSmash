const int out[3][3]={
  {49,41,25},
  {50,42,26},
  {52,44,28}
};
  
bool hab[3][3]={
{0,1,0},
{1,1,1},
{0,1,0},
};
volatile int fila=0,col=0;

void setup() {
 
  DDRC=63;
  PORTC=56;
  Serial.begin(9600);
  cli();                              //deshabilito temporalmente las interrupciones
  TCCR1A=0;                           //limpiamos los registros de control
  TCCR1B=0;                           //del timer 1

  OCR1A=30;            //3 valor minimo, 5 se ve multiplexado en grabacion de 60fps, 8 valor maximo
  TCCR1B|=(1<<WGM12);
  TCCR1B|=(1<<CS10);
  TCCR1B|=(1<<CS12);
  
  
  TIMSK1|=(1<<OCIE1A);                  //enciendo el timer1
  
  sei();                              //activamos las interrupciones
  
}

void loop() {

}

ISR(TIMER1_COMPA_vect){                 //ISR es la rutina de interrupcion
  if(hab[fila][col]){
    PORTC=out[fila][col];
  }
  else {
    PORTC=0;
  }
  
  if(fila<2)fila++;
  else{
    fila=0;
    if(col<2)col++;
    else col=0;
  }
}
