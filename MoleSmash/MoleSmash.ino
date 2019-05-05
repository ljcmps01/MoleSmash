/*ESTADO ACTUAL
 * Cada aproximadamente 1 segundo o si se toca un boton se obtendran dos valores random, los cuales diran que led se prenden
 * HACER:variable de puntaje base, si era el correcto subir un punto, si no lo era, restarlo
 * Idea botonera con keypad, usar un bloque switch para determinar los valores x;y presionados de acuerdo con la tecla presionada
 */
#include <Key.h>
#include <Keypad.h>

#define COL PORTK
#define FILA PORTF
#define MaxVidas 3
const byte rowsCount = 3;
const byte columsCount = 3;
 
char keys[rowsCount][columsCount] = {
   { '1','2','3' },
   { '4','5','6' },
   { '7','8','9' },

};

const byte rowPins[rowsCount] = { 11, 10, 9 };
const byte columnPins[columsCount] = { 7, 6, 5 };

volatile int c_Seg=1953;   //Cuando llegue a 1953 habrá llegado al segundo
volatile int r1,r2;     //variables random
const int out[7]={1,2,4,8,16,32,64};
bool hab[3][3]={
{0,0,0},
{0,0,0},
{0,0,0},
};
volatile int fila=0,col=0;

void getRand();

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

void edit(char tecla);

bool wait=0;
volatile int score=0;
volatile int vidas=MaxVidas;
volatile bool start=0;
volatile int numeros=0;
volatile int nivel[10]={1750,1500,1250,1000,900,800,750,600,500,400};
volatile int vel;
volatile int seed;
void setup() {
 
  DDRF=255;
  FILA=0;  
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
  char key = keypad.getKey();
  
  if (key) {
    seed=c_Seg;
    if(!start){
      Begin();
    }
    else edit(key);
  }  
}

ISR(TIMER1_COMPA_vect){                 //ISR es la rutina de interrupcion
  if(start){
    if(c_Seg==0){
      getRand();
      if(numeros<=100)vel=numeros/10;
      else vel=9;
      c_Seg=nivel[vel];
      numeros++;
      
      Serial.print("Numeros: ");
      Serial.println(numeros);      
      Serial.print("Velocidad: ");
      Serial.println(vel);
      wait=0;
    }
    else{
      c_Seg--;
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
}

void getRand(){
  randomSeed(seed);
  hab[r1][r2]=0;
  r1=random(3);
  r2=random(3);
  hab[r1][r2]=1;
}

void edit(char tecla){
  wait=1;
  Serial.println(tecla);
  switch(tecla){
    
    case 49:
      if(hab[0][0]){
      hab[0][0]=0;
      score++;
      }
      else vidas--;
      break;
    case 50:
      if(hab[0][1]){
      hab[0][1]=!hab[0][1];
      score++;
      }
      else vidas--;
      break;
    case 51:
      if(hab[0][2]){
      hab[0][2]=!hab[0][2];
      score++;
      }
      else vidas--;
      break;
    case 52:
      if(hab[1][0]){
      hab[1][0]=!hab[1][0];
      score++;
      }
      else vidas--;
      break;
    case 53:
      if(hab[1][1]){
      hab[1][1]=!hab[1][1];
      score++;
      }
      else vidas--;
      break;
    case 54:
      if(hab[1][2]){
      hab[1][2]=!hab[1][2];
      score++;
      }
      else vidas--;
      break;
    case 55:
      if(hab[2][0]){
      hab[2][0]=!hab[2][0];
      score++;
      }
      else vidas--;
      break;
    case 56:
      if(hab[2][1]){
      hab[2][1]=!hab[2][1];
      score++;
      }
      else vidas--;
      break;
    case 57:
      if(hab[2][2]){
      hab[2][2]=!hab[2][2];
      score++;
      }
      else vidas--;
      break;
    default:break;
  }
  Serial.print("Score: ");
  Serial.println(score);
  Serial.print("Vidas: ");
  Serial.println(vidas);
  if(vidas==0)start=0;
}

void Begin(){
  start=1;
  vidas=MaxVidas;
  numeros=0;
}
