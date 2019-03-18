#include <SoftwareSerial.h>
 
#define DEBUG true
 
SoftwareSerial modulo(2,3); // Pin 2 receptor, Pin 3 transmisor

void setup() {
  Serial.begin(115200);
  modulo.begin(115200);
  String string = "";
  char c = "";
  delay(1000);
  modulo.print("AT+CIOBAUD=9600\r\n");
  
  Serial.begin(9600);
  modulo.begin(9600);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
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

void loop(){
  if(modulo.available()){   // revisar si hay mensaje del ESP8266
    if(modulo.find("+IPD,")){ // revisar si el servidor recibio datos
      delay(1500); // esperar que lleguen los datos hacia el buffer
      int conexionID = modulo.read()-48; // obtener el ID de la conexión para poder responder
      modulo.find("led="); // bucar el texto "led="
      int state = (modulo.read()-48); // Obtener el estado del pin a mostrar
      digitalWrite(13, state); // Cambiar estado del pin
      while(modulo.available()){
        char c = modulo.read();
        Serial.print(c);
      }
      //responder y cerrar la conexión para que el navegador no se quede cargando 
      // página web a enviar
      String webpage = "";
      if (state==1)
        webpage += "<h1>LED_13 = encendido!</h1>";
      else {
        webpage += "<h1>LED_13 = apagado!</h1>";
      }
      // comando para enviar página web
      String comandoWebpage = "AT+CIPSEND=";
      comandoWebpage+=conexionID;
      comandoWebpage+=",";
      comandoWebpage+=webpage.length();
      comandoWebpage+="\r\n";
      sendData(comandoWebpage,1000);
      sendData(webpage,1000);
      // comando para terminar conexión
      String comandoCerrar = "AT+CIPCLOSE=";
      comandoCerrar+=conexionID;
      comandoCerrar+="\r\n";
      sendData(comandoCerrar,3000);
    }
  }
}

void sendData(String comando, const int timeout){
  long int time = millis(); // medir el tiempo actual para verificar timeout
  modulo.print(comando); // enviar el comando al ESP8266
  while( (time+timeout) > millis()){ //mientras no haya timeout
    while(modulo.available()){ //mientras haya datos por leer 
      // Leer los datos disponibles
      char c = modulo.read(); // leer el siguiente caracter
      Serial.print(c);
    }
  } 
  return;
}
