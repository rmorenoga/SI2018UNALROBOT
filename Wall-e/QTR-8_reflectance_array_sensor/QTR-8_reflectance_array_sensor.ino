#include <QTRSensors.h>

QTRSensorsRC qtrrc((unsigned char[]) { A5,A4,A3,A2,A1,A0} ,6, 2500, 2);
unsigned int values[5];

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  calibrateSensors();
}

void loop() {
  // put your main code here, to run repeatedly:
  getValues();
  for (int i = 0; i<6;i++){
    Serial.print(values[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(500);
}

void getValues(){
//  qtrrc.read(values);
    values[0] = analogRead(A0);
    values[1] = analogRead(A1);
    values[2] = analogRead(A2);
    values[3] = analogRead(A3);
    values[4] = analogRead(A4);
    values[5] = analogRead(A5);
}

void calibrateSensors(){
  for (int i = 0; i < 200; i++)
  {
  qtrrc.calibrate(QTR_EMITTERS_ON);
  delay(20);
  }
//  for (int i = 0; i < 6; i++)
//  {
//  Serial.print(qtrrc.calibratedMinimumOn[i]);
//  Serial.print(' ');
//  }
//  Serial.println();
//
//  delay(2000);
//    
//  for (int i = 0; i < 6; i++)
//  {
//  Serial.print(qtrrc.calibratedMaximumOn[i]);
//  Serial.print(' ');
//  }
//  Serial.println();
//  Serial.println();
}
