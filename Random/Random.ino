/*En un futuro se utilizará la funcion randomseed() para volver el juego verdaderamente aleatorio
 * Se colocara un contador que empezará a funcionar apenas se conecte el arduino
 * una vez comenzado el juego, se tomará el valor del contador como el argumento de randomseed
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println(random(5));
  delay(1000);
}
