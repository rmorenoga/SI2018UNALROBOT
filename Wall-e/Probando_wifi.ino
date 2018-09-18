#include <SoftwareSerial.h>
#define AvanzarLed 5
#define RetrocederLed 6

SoftwareSerial modulo(3,2); // Pin 0 receptor, Pin 1 transmisor

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
          digitalWrite(AvanzarLed, HIGH);
          delay(2000);
          digitalWrite(AvanzarLed, LOW);
        }else if( message == "Retroceder" ){
          Serial.print("Retrocediendo\n");
          digitalWrite(RetrocederLed, HIGH);
          delay(2000);
          digitalWrite(RetrocederLed, LOW);
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
  pinMode(AvanzarLed, OUTPUT);
  pinMode(RetrocederLed, OUTPUT);
}
