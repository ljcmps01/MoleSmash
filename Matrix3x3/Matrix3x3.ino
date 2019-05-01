#define COL PORTK
#define FILA PORTF

const int out[7]={1,2,4,8,16,32,64};
bool hab[3][3]={
{1,1,1},
{1,0,1},
{1,1,1},
};
volatile int fila=0,col=0;

void setup() {
 
  DDRF=255;
  FILA=1;  
  DDRK=255;
  COL=254;
  Serial.begin(9600);
  cli();                              //deshabilito temporalmente las interrupciones
  TCCR1A=0;                           //limpiamos los registros de control
  TCCR1B=0;                           //del timer 1

  OCR1A=8;            //3 valor minimo, 5 se ve multiplexado en grabacion de 60fps, 8 valor maximo
  TCCR1B|=(1<<WGM12);
  TCCR1B|=(1<<CS10);
  TCCR1B|=(1<<CS12);
  
  
  TIMSK1|=(1<<OCIE1A);                  //enciendo el timer1
  
  sei();                              //activamos las interrupciones

}

void loop() {

}

ISR(TIMER1_COMPA_vect){                 //ISR es la rutina de interrupcion
  if(hab[fila][col])
  COL=~(1<<col);
  else COL=~(0<<col);
  FILA=(1<<fila);
  if(col<2)col++;
  else{
    col=0;
    if(fila<2)fila++;
    else fila=0;
  }
}
