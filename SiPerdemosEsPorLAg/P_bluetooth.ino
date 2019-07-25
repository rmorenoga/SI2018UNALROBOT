#include <SoftwareSerial.h>

SoftwareSerial modulo(2,3);

void setup() {

  modulo.begin(9600);
  
  delay(1000);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop(){
  String cadena = modulo.readString();
  if(cadena == "1"){
    digitalWrite(13,!digitalRead(13));
  }
}
