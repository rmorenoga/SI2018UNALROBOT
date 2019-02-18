#include <SoftwareSerial.h>

SoftwareSerial modulo(3,2); // Pin 0 receptor, Pin 1 transmisor

int IN1=6;
int IN2=9;
int IN3=10;
int IN4=11;
int value_of_speed=40;

void setup() {
  configPins();
  Serial.begin(9600);
  modulo.begin(9600);
  String string = "";
  char c = "";
  delay(1000);
  modulo.print("AT+CIPMUX=1\r\n");
  delay(1000);
  modulo.print("AT+CIPSERVER=1,80\r\n");
  delay(1000);
  while(modulo.available()>0){
    c = modulo.read();
    string += c;
  }
  string += "\n";
  Serial.print(string);
}

// the loop function runs over and over again forever
void loop() {
  String string = "";
  String message = "";
  char c = "";
  bool signalMessage = false;
  while(modulo.available()>0){
    c = modulo.read();
    string += c;
    if( signalMessage == true && (String)c != " " ){
      message += c;
    }else{
      signalMessage = false;
      if (message != ""){
        if( message == "Avanzar" ){
          Serial.print("Avanzando\n");
          avanzar();
        }else if( message == "Retroceder" ){
          Serial.print("Retrocediendo\n");
          retroceder();
        }else if( message == "Derecha" ){
          Serial.print("Girando a la derecha\n");
          derecha();
        }else if( message == "Izquierda" ){
          Serial.print("Girando a la izquierda\n");
          izquierda();
        }
        message += "\n";
        Serial.print(message);
        message = "";
      }
    }
    if( (String)c == "/" && (String)string[string.length()-2] == " " ){
      signalMessage = true;
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
  analogWrite(IN1,0);
  analogWrite(IN2,value_of_speed);
  analogWrite(IN3,value_of_speed);
  analogWrite(IN4,0);

  delay(500);

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}

void retroceder(){
  analogWrite(IN1,value_of_speed);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,value_of_speed);

  delay(500);

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}

void derecha(){
  analogWrite(IN1,0);
  analogWrite(IN2,value_of_speed);
  analogWrite(IN3,0);
  analogWrite(IN4,value_of_speed);

  delay(500);

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}

void izquierda(){
  analogWrite(IN1,value_of_speed);
  analogWrite(IN2,0);
  analogWrite(IN3,value_of_speed);
  analogWrite(IN4,0);

  delay(500);

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}
