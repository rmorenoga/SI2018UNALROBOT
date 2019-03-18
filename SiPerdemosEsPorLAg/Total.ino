#include <QTRSensors.h>
#include <SoftwareSerial.h>

// QTR_NO_EMITTER_PIN.

#define NUM_SENSORS   7     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   4     // emitter is controlled by digital pin 2 
#define DEBUG true

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {A0,A1,A2,A3,A4,A5,9},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
 
SoftwareSerial modulo(2,3); // Pin 2 receptor, Pin 3 transmisor

int IN1=5;
int IN2=6;
int IN3=10;
int IN4=11;
int num;
String cadena;

void setup() {
  delay(500);//Calibra los sensores
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }//Finaiza la calibración.
  
  configPins();// Configura los pines para darle movimiento al robot
  modulo.begin(115200);//Inicia la comunicación con el modulo Wi-Fi
  String string = "";
  char c = "";
  delay(1000);
  modulo.print("AT+CIOBAUD=9600\r\n");
  
  Serial.begin(9600);
  
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
  
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

  qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
}

void loop(){
  if(modulo.available()){   // revisar si hay mensaje del ESP8266
    if(modulo.find("+IPD,")){ // revisar si el servidor recibio datos
      delay(1500); // esperar que lleguen los datos hacia el buffer
      int conexionID = modulo.read()-48; // obtener el ID de la conexión para poder responder
      modulo.find("led="); // bucar el texto "led="
      int state = (modulo.read()-48); // Obtener el estado del pin a mostrar
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
      }
      qtrrc.readLine(sensorValues);

      // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
      // 1000 means minimum reflectance, followed by the line position
      cadena = "";
      for ( unsigned char i = 0; i < NUM_SENSORS; i++ ){
        Serial.print(sensorValues[i]);
        Serial.print('\t');
        num = (sensorValues[i]<500)?0:1;
        cadena += ( ( String )num ) + " ";
      }
      Serial.println(); // uncomment this line if you are using raw values
      
      while(modulo.available()){
        char c = modulo.read();
        Serial.print(c);
      }
      //responder y cerrar la conexión para que el navegador no se quede cargando 
      // página web a enviar
      String webpage = "";
      if (state==1)
        webpage += "<h1>" + cadena + "<br>Avanzando</h1>";
      else if (state==2)
        webpage += "<h1>" + cadena + "<br>Retrocediendo</h1>";
      else if (state==3)
        webpage += "<h1>" + cadena + "<br>Girando a la derecha</h1>";
      else if (state==4)
        webpage += "<h1>" + cadena + "<br>Girando a la izquierda</h1>";
      else{
        webpage += "<h1>Nada</h1>";
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

void configPins() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void avanzar(){
  analogWrite(IN1,37);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,27);

  delay(470);

  apagarMotores();
}

void retroceder(){
  analogWrite(IN1,0);
  analogWrite(IN2,37);
  analogWrite(IN3,28);
  analogWrite(IN4,0);

  delay(520); //delay 2 seconds

  apagarMotores();
}

//Giro de 90 Grados hacia la Derecha
void derecha(){
  analogWrite(IN1,0);
  analogWrite(IN2,32);
  analogWrite(IN3,0);
  analogWrite(IN4,27);

  delay(580);

  apagarMotores();
}
//Giro de 90 Grados hacia la izquierda
void izquierda(){
  analogWrite(IN1,32);
  analogWrite(IN2,0);
  analogWrite(IN3,27);
  analogWrite(IN4,0);

  delay(580);

  apagarMotores();
}

void apagarMotores(){
  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
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
