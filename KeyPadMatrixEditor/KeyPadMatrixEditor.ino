/*
 * Este programa es capaz de prender o apagar los led de la matriz 
 * utilizando los botones del teclado matricial
 */
#include <Key.h>
#include <Keypad.h>

#define COL PORTK
#define FILA PORTF

const byte rowsCount = 3;
const byte columsCount = 3;
 
char keys[rowsCount][columsCount] = {
   { '1','2','3' },
   { '4','5','6' },
   { '7','8','9' },

};

const byte rowPins[rowsCount] = { 11, 10, 9 };
const byte columnPins[columsCount] = { 7, 6, 5 };

const int out[7]={1,2,4,8,16,32,64};
bool hab[3][3]={
{1,1,1},
{1,1,1},
{1,1,1},
};
volatile int fila=0,col=0;

 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

void edit(char tecla);

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
  
  sei();                              //activamos las interrupciones}
}
void loop() {
   char key = keypad.getKey();
 
   if (key) {
      Serial.println(key);
      edit(key);
   }
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
void edit(char tecla){
  
  Serial.println(tecla);
  switch(tecla){
    
    case 49:
      if(hab[0][0])
      hab[0][0]=0;
      break;
    case 50:
      if(hab[0][1])
      hab[0][1]=!hab[0][1];
      break;
    case 51:
      if(hab[0][2])
      hab[0][2]=!hab[0][2];
      break;
    case 52:
      if(hab[1][0])
      hab[1][0]=!hab[1][0];
      break;
    case 53:
      if(hab[1][1])
      hab[1][1]=!hab[1][1];
      break;
    case 54:
      if(hab[1][2])
      hab[1][2]=!hab[1][2];
      break;
    case 55:
      if(hab[2][0])
      hab[2][0]=!hab[2][0];
      break;
    case 56:
      if(hab[2][1])
      hab[2][1]=!hab[2][1];
      break;
    case 57:
      if(hab[2][2])
      hab[2][2]=!hab[2][2];
      break;
    default:break;
  }
}
