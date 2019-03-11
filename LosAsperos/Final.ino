#include <SoftwareSerial.h>
#include <QTRSensors.h>

#define NUM_SENSORS   6     // numero de sensores usados
#define TIMEOUT       2500  // esperar 2.5 ms para tener una respuesta del sensado
#define EMITTER_PIN   3     // este pin controla el led on del los sensores (enciende y apaga)
  
//aqui se pone el numero de los pines conectados a los sensores
QTRSensorsRC qtrrc((unsigned char[]) {A0,A1,A2,A3,A4,A5}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

SoftwareSerial mySerial(13, 12); // RX, TX
  
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  setupWiFi();
  setupSensors();  
}

void loop() {
  //----------------------------------------SENSORS----------------------------------------
  unsigned int position = qtrrc.readLine(sensorValues);  
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }  
  Serial.println(position); // comment this line out if you are using raw values
    
  delay(50);
  
  //---------------------------------------- WIFI ------------------------------------------
   String IncomingString="";
   boolean StringReady = false;
 
 while (mySerial.available()){
   IncomingString=mySerial.readString();
   StringReady= true;
  }

  if (StringReady){
    Serial.println("Received String: " + IncomingString);
  }  
  if (IncomingString.indexOf("AV") != -1) {
    digitalWrite(11,HIGH);
    digitalWrite(8,HIGH);
   }
  if (IncomingString.indexOf("STOP") != -1) {
    digitalWrite(11,LOW);
    digitalWrite(8,LOW);
   }     
}

boolean SendCommand(String cmd, String ack){
  mySerial.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
 
boolean echoFind(String keyword){
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  if (mySerial.available()){
    char ch = mySerial.read();
    Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println();
       return true;
    }
   }
  }
 return false; // Timed out
}

void setupSensors()
{
  delay(500);  
  for (int i = 0; i < 200; i++)  // la calibracion se lleva a cabo por 5 segundos 
  {
    qtrrc.calibrate();       // funcion para calibrar los sensores
  }
  delay(50);  
  
  // imprime la calibracion minima de los sensores  
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
    
  // imprime la calibracion maxima de los sensores
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void setupWiFi(){
  SendCommand("AT","OK");
  //SendCommand("AT+CIOBAUD=9600", "OK");
  //SendCommand("AT+CWMODE=2","OK"); //Para que acceder al robot como AP
  SendCommand("AT+CWMODE=1","OK"); // Para que el robot sea parte de una red local
  SendCommand("AT+CWJAP=\"CC_Family\",\"daniel25\"","OK");
  SendCommand("AT+RST", "Ready");  ;
  SendCommand("AT+CIFSR", "OK");  
  SendCommand("AT+CIPMUX=1","OK");
  SendCommand("AT+CIPSERVER=1,80","OK");
  SendCommand("AT+CIPMODE?","OK");
  SendCommand("AT","OK");  
}
 
