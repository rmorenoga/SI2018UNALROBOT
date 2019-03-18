#include <QTRSensors.h>
#define NUM_SENSORS   6     // numero de sensores usados
#define TIMEOUT       2500  // esperar 2.5 ms para tener una respuesta del sensado
#define EMITTER_PIN   3     // este pin controla el led on del los sensores (enciende y apaga)
  
//aqui se pone el numero de los pines conectados a los sensores
QTRSensorsRC qtrrc((unsigned char[]) {A0,A1,A2,A3,A4,A5},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
  
  
void setup()
{
  delay(500);
  pinMode(13, OUTPUT);        
  digitalWrite(13, HIGH);    //este led encendido indica que comienza la calibracion
  for (int i = 0; i < 200; i++)  // la calibracion se lleva a cabo por 5 segundos 
  {
    qtrrc.calibrate();       // funcion para calibrar los sensores
  }
  digitalWrite(13, LOW);     // se paga el led para que indique que termino la calibracion
  
  // imprime la calibracion minima de los sensores
  Serial.begin(9600);
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
    
  delay(50);
}
