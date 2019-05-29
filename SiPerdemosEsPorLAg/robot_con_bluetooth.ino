#include <SoftwareSerial.h>

SoftwareSerial s(10,11);
void setup() {
  // put your setup code here, to run once:
  s.begin(9600);
  s.println("hola mundo");
}

void loop() {
  // put your main code here, to run repeatedly:
  String cadena = s.readString();
  if(cadena == "hola")
    s.println("que mas?");
    
}
