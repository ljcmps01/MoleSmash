/*ESTADO ACTUAL
 * Cada aproximadamente 1 segundo se obtendran dos valores random, los cuales diran que led se prenden
 * 
 * Idea botonera con keypad, usar un bloque switch para determinar los valores x;y presionados de acuerdo con la tecla presionada
 */

#define COL PORTK
#define FILA PORTF

volatile int c_Seg=0;   //Cuando llegue a 1953 habrá llegado al segundo
volatile int r1,r2;     //variables random
const int out[7]={1,2,4,8,16,32,64};
bool hab[3][3]={
{0,0,0},
{0,0,0},
{0,0,0},
};
volatile int fila=0,col=0;

void getRand();

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
  if(c_Seg<=1953){
    c_Seg++;
  }
  else{
    getRand();
    c_Seg=0;
  }

  
  //Multiplexado de la matriz
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

void getRand(){
  hab[r1][r2]=0;
  r1=random(3);
  r2=random(3);
  hab[r1][r2]=1;
}
