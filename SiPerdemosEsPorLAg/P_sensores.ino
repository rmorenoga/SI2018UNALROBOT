#include <QTRSensors.h>

#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   4

QTRSensorsRC qtrrc((unsigned char[]) {A0,9,A1,A2,A3,A4,7,A5},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

void setup() {
  delay(500);
  for (int i = 0; i < 400; i++)
  {
    qtrrc.calibrate();
  }

  delay(1000);
  
  Serial.begin(9600);
  
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

}

void loop(){
  qtrrc.readLine(sensorValues);

  String cadena = "";
  for ( unsigned char i = 0; i < NUM_SENSORS; i++ ){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
    cadena += ( ( String )sensorValues[i] ) + " ";
  }
  Serial.println();

  delay(200);

}
