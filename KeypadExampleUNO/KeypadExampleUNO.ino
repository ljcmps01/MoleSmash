#include <Key.h>
#include <Keypad.h>

const byte rowsCount = 3;
const byte columsCount = 3;
 
char keys[rowsCount][columsCount] = {
   { '1','2','3' },
   { '4','5','6' },
   { '7','8','9' },

};
 
const byte rowPins[rowsCount] = {10, 9,8 };
const byte columnPins[columsCount] = { 13, 12, 11 };
 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);
 
void setup() {
   Serial.begin(9600);
}
 
void loop() {
   char key = keypad.getKey();
 
   if (key) {
      Serial.println(key);
   }
}
