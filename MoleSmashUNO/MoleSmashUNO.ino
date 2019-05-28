/*Puerto C Leds
 *Puerto B pulsadores 
 *Puerto D:
 *  pines 0 y 1, no conectados
 *  pines 2,3,4 5 al ABCD del 4511
 *  pines 6 y 7 transistores de Multiplexado
 */
#include <Key.h>
#include <Keypad.h>

#define MaxVidas 3        //Modificar el numero segun el numero de vidas que se le quiera otorgar al jugador

//Variables de los botones
const byte rowsCount = 3; 
const byte columsCount = 3;

//Valores del teclado 
char keys[rowsCount][columsCount] = {
   { '1','2','3' },
   { '4','5','6' },
   { '7','8','9' },
};

//Pines donde se encuentran conectados el teclado
const byte rowPins[rowsCount] = {10, 9,8 };
const byte columnPins[columsCount] = { 13, 12, 11 };

//Salidas de la matriz
const int out[3][3]={
  {49,41,25},
  {50,42,26},
  {52,44,28}
};

volatile int c_Seg=1953;   //Cuando llegue a 1953 habrá llegado al segundo
volatile int r1,r2;     //variables random

//Matriz virtual representante de los leds
bool hab[3][3]={
{0,0,0},
{0,0,0},
{0,0,0},
};
volatile int fila=0,col=0;

//Funcion generadora de numeros aleatorios
void getRand();

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

//Funcion de control de la matriz led con los pulsadores
void edit(char tecla);

//Variables del juego
bool wait=0;
volatile int score=0; //Puntaje
volatile int vidas=MaxVidas;//Vidas
volatile bool start=0;//Comienzo de juego
volatile int numeros=0;//Cantidad de numeros mostrados
volatile int nivel[10]={1750,1500,1250,1000,900,800,750,600,500,400};//Control de velocidad
volatile int vel;
volatile int seed;//Semilla de numeros aleatorios

int Display(int puntos,bool digito);//Funcion con retorno para los display
volatile bool m_Display=0;//Variable de multiplexacion del display

void setup() {
  //Configuracion de los puertos
  DDRD|=(63<<2);
  DDRC=63;
  PORTC=56;
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

//Funcion de deteccion y accion de los pulsadores 
void loop() {
  char key = keypad.getKey();
  
  if (key) {
    randomSeed(c_Seg);
    if(!start){//Si todavia no comenzo el juego se inicia
      Begin();  //Se resetean los valores a los iniciales
    }
    else edit(key);//Si ya inicio se llama a la funcion edit
  }  
}

ISR(TIMER1_COMPA_vect){                 //ISR es la rutina de interrupcion
  if(start){
    if(c_Seg==0){//Cuando el contador c_Seg llegue a 0
      getRand();//Se obtienen 2 numeros random que indicaran que led se prenderá
      if(numeros<=100)vel=numeros/10;//Se calcula la velocidad y nivel del juego en base a los numeros mostrados, cada 10 numeros se aumenta la velocidad
      else vel=9;//En caso de llegar al ultimo nivel se mantiene en este para evitar que el juego deje de funcionar
      c_Seg=nivel[vel];
      numeros++;
      wait=0;
    }
    else{
      c_Seg--;
    }
    
    //Multiplexado de la matriz
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

    //Multiplexado de displays
    m_Display=!m_Display;
    if(m_Display){
      PORTD=(Display(score,m_Display)<<2);        
      PORTD|=(1<<6);
    }
    else{
      PORTD=(Display(score,m_Display)<<2);
      PORTD|=(1<<7);
    }
  }
}

void getRand(){
  hab[r1][r2]=0;
  r1=random(3);
  r2=random(3);
  hab[r1][r2]=1;
}

//control de los leds en base a los pulsador
void edit(char tecla){
  wait=1;
  //Se lee la tecla presionada 
  switch(tecla){
    //Si el boton presionado coincide con el led prendido, se apaga y pasa a mostrar otro numero
    //Si el boton presionado coincide con un led apagado, el jugador pierde una vida
    case 49://se presiono el 1
      if(hab[0][0]){
      hab[0][0]=0;
      score++;
      }
      else vidas--;
      break;
    case 50://se presiono el 2
      if(hab[0][1]){
      hab[0][1]=!hab[0][1];
      score++;
      }
      else vidas--;
      break;
    case 51://se presiono el 3
      if(hab[0][2]){
      hab[0][2]=!hab[0][2];
      score++;
      }
      else vidas--;
      break;
    case 52://se presiono el 4
      if(hab[1][0]){
      hab[1][0]=!hab[1][0];
      score++;
      }
      else vidas--;
      break;
    case 53://se presiono el 5
      if(hab[1][1]){
      hab[1][1]=!hab[1][1];
      score++;
      }
      else vidas--;
      break;
    case 54://se presiono el 6
      if(hab[1][2]){
      hab[1][2]=!hab[1][2];
      score++;
      }
      else vidas--;
      break;
    case 55://se presiono el 7
      if(hab[2][0]){
      hab[2][0]=!hab[2][0];
      score++;
      }
      else vidas--;
      break;
    case 56://se presiono el 8
      if(hab[2][1]){
      hab[2][1]=!hab[2][1];
      score++;
      }
      else vidas--;
      break;
    case 57://se presiono el 9
      if(hab[2][2]){
      hab[2][2]=!hab[2][2];
      score++;
      }
      else vidas--;
      break;
    default:break;
  }
  //Muestra de las vidas restantes y los puntos en el monitor serial
  Serial.print("Score: ");
  Serial.println(score);
  Serial.print("Vidas: ");
  Serial.println(vidas);

  //Si el jugador se queda sin vidas se apaga el juego
  if(vidas==0)start=0;
}

void Begin(){
  start=1;
  vidas=MaxVidas;
  numeros=0;
  score=0;
}

int Display(int puntos,bool digito){
  if(digito){
    return puntos/10;
  }
  else{
    return puntos%10;
  }
}
