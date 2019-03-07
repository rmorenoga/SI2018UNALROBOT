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
int value_of_speed12=50;
int value_of_speed34=40;
int turning_speed12=40;
int turning_speed34=30;

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


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values
  
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
  delay(250);
  
}

void configPins() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void avanzar(){
  analogWrite(IN1,0);
  analogWrite(IN2,value_of_speed12);
  analogWrite(IN3,value_of_speed34);
  analogWrite(IN4,0);

  delay(500); //delay 2 seconds

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}

void retroceder(){
  analogWrite(IN1,value_of_speed12);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,value_of_speed34);

  delay(500);

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}
void derecha(){
  analogWrite(IN1,0);
  analogWrite(IN2,turning_speed34);
  analogWrite(IN3,0);
  analogWrite(IN4,turning_speed12);

  delay(250);
  
  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}

void izquierda(){
  analogWrite(IN1,turning_speed12);
  analogWrite(IN2,0);
  analogWrite(IN3,turning_speed34);
  analogWrite(IN4,0);

  delay(250);

  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}
