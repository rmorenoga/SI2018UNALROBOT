#include "WiFiEsp.h"
#include <SoftwareSerial.h>
#include <QTRSensors.h>


char ssid[] = "CC_Family";       // your network SSID (name)
char pass[] = "daniel25";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "192.168.0.8";


#define NUM_SENSORS   7     // numero de sensores usados
#define TIMEOUT       2500  // esperar 2.5 ms para tener una respuesta del sensado
#define EMITTER_PIN   3     // este pin controla el led on del los sensores (enciende y apaga)
  
//aqui se pone el numero de los pines conectados a los sensores
QTRSensorsRC qtrrc((unsigned char[]) {A0,A1,A2,A3,A4,A5, 2}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
unsigned int percepts[NUM_SENSORS];

SoftwareSerial Serial1(13, 12); // RX, TX

char req[255] = "";

void setup() {
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  printWifiStatus();
  setupSensors();  
  
}

int state = 1; // 0 Espera - 1 Percibir - 2 Actuar
void loop() {
  // put your main code here, to run repeatedly:
  if(state == 1){
    WiFiEspClient client;
    percibir();
    Serial.println(req);
    Serial.println("Starting connection to server...");
    // if you get a connection, report back via serial
    if (client.connect(server, 443)) {
      Serial.println("Connected to server");
      // Make a HTTP request
      client.println(req);
      client.println("Host: 192.168.0.8");
      client.println("Connection: close");
      client.println();
      client.stop();
      delay(1000);
    }
    Serial.println("Sent");
    state = 2;
  }
  
  if(state == 2){
    WiFiEspServer server(80);
    server.begin();
    WiFiEspClient client = server.available();
    
    if (client) {
      Serial.println("New client");
      WiFiEspClient cliente;
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();        
          Serial.write(c);                           
          mover(c);
          if(state == 1){
            client.stop();
            delay(1000);
          }                    
        }
      }
      // give the web browser time to receive the data
      delay(10);      
      // close the connection:
      Serial.println("Client disconnected");
    }
  }
}

void mover(char instr){
  bool search = true;

  switch(instr){
    case '0':
      digitalWrite(11, HIGH);
      digitalWrite(8, HIGH);      
    break;
    case '1':
      digitalWrite(10, HIGH);
      digitalWrite(8, HIGH);
      delay(1000);
      digitalWrite(10, LOW);
      digitalWrite(8, LOW);
      delay(250);
      digitalWrite(11, HIGH);
      digitalWrite(8, HIGH);
    break;
    case '2':
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);      
    break;
    case '3':
      digitalWrite(11, HIGH);
      digitalWrite(9, HIGH);
      delay(1000);
      digitalWrite(11, LOW);
      digitalWrite(9, LOW);
      delay(250);
      digitalWrite(11, HIGH);
      digitalWrite(8, HIGH);
    break;
    default:
    return;
  }
  
  while(search){
    unsigned int position = qtrrc.readLine(sensorValues);      
    if(sensorValues[6] < 500){
      search = false;
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);
      digitalWrite(11, LOW);
      digitalWrite(8, LOW);
      state = 1;
    }
    delay(50);
  } 

}

void percibir(){    
  strcpy(req, "");
  char server[] = "192.168.0.8";  
  getSensors();

  char s1[20] = "GET /send-percepts?";
  char f[20] = "front=";
  char r[20] = "&right=";
  char b[20] = "&back=";
  char l[20] = "&left="; 
  char tr[20] = "&treasure=";
  char fa[20] = "&fail=";
  char s2 = " HTTP/1.1";

  
  strcat(req, s1);
  
  if(percepts[0] < 500){
    strcat(f, "false");
  }else{
    strcat(f, "true");
  }

  if(percepts[1] < 500){
    strcat(l, "false");
  }else{
    strcat(l, "true");
  }

  if(percepts[4] < 500){
    strcat(r, "false");
  }else{
    strcat(r, "true");
  }

  if(percepts[5] < 500){
    strcat(b, "false");
  }else{
    strcat(b, "true");
  }
  
  strcat(req, f);
  strcat(req, l);
  strcat(req, r);
  strcat(req, b);
  
}

void getSensors( ) {      
   int tics = 16;
   
   for(int row = 0; row < tics; row++){
    unsigned int position = qtrrc.readLine(sensorValues);  
     for (int i = 0; i < NUM_SENSORS; i++){        
        Serial.print(sensorValues[i]);
        Serial.print('\t');
        percepts[i] += sensorValues[i];
     }  
     Serial.println(position); // comment this line out if you are using raw values    
     delay(50);
   }  

   for (int i = 0; i < NUM_SENSORS; i++){                
        percepts[i] = percepts[i]/tics;
     }  
   
}

void setupSensors(){
  Serial.println("Calibrando Sensores...");
  delay(500);  
  for (int i = 0; i < 200; i++)  // la calibracion se lleva a cabo por 5 segundos 
  {
    qtrrc.calibrate();       // funcion para calibrar los sensores
  }
  delay(50);  
  
  // imprime la calibracion minima de los sensores  
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
    
  // imprime la calibracion maxima de los sensores
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
  Serial.println("Sensores Listos.");
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
