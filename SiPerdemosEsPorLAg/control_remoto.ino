#include <SoftwareSerial.h>

#define DEBUG true
 
SoftwareSerial modulo(2,3);

int IN1=5;
int IN2=6;
int IN3=10;
int IN4=11;

void setup() {
  
  configPins();
  modulo.begin(9600);
  String string = "";
  delay(1000);
  
  Serial.begin(9600);
  
  modulo.begin(9600);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop(){
  String cadena = modulo.readString();
  if(cadena == "1" || cadena == "2" || cadena == "3" || cadena == "4" || cadena == "5"){
    int state = cadena.toInt();
    switch( state ){
      case 1:
        Serial.print("Avanzando\n");
        avanzar();
        break;
      case 2:
        Serial.print("Retrocediendo\n");
        retroceder();
        break;
      case 3:
        Serial.print("Derecha\n");
        derecha();
        break;
      case 4:
        Serial.print("Izquierda\n");
        izquierda();
        break;
      case 5:
        Serial.print("Parar\n");
        parar();
        break;
    }
  }
}

void configPins() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void avanzar(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, 0);
  analogWrite(IN2, 60);
  analogWrite(IN3, 60);
  analogWrite(IN4, 0);
}

void retroceder(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, 60);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 60);
}

void derecha(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, 0);
  analogWrite(IN2, 80);
  analogWrite(IN3, 0);
  analogWrite(IN4, 80);
}

void izquierda(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, 70);
  analogWrite(IN2, 0);
  analogWrite(IN3, 70);
  analogWrite(IN4, 0);
}

void parar(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}
