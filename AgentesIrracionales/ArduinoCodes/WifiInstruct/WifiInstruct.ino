
#include <QTRSensors.h>

#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   6     // emitter is controlled by digital pin 6
#define KP 2
#define KD 5

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {14, 15, 16, 17, 18, 19}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
int IN1 = 4, IN2 = 5, IN3 = 3, IN4 = 2;
int ENALeft = 9, ENARight = 10;
int TurnSpeed = 80, ForwardSpeed = 50;

void CalibrateSensor(){
   delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
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
}

void InitializeEngine(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENALeft, OUTPUT);
  pinMode(ENARight, OUTPUT);
}

void Atras(int FwlSignal, int FwrSignal){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENALeft, FwlSignal);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENARight, FwrSignal);
}

void Adelante(int FwlSignal, int FwrSignal){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENALeft, FwlSignal);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENARight, FwrSignal);  
}

void Derecha(int TrSignal){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENALeft, TrSignal);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENARight, TrSignal);  
}

void Izquierda(int TrSignal){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENALeft, TrSignal);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENARight, TrSignal);  
}

void Stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
}

void EngineTest(){
  Adelante(ForwardSpeed, ForwardSpeed);
  delay(1000);
  Stop();
  delay(500);
  Atras(ForwardSpeed, ForwardSpeed);
  delay(1000);
  Stop();
  delay(500);
  Izquierda(TurnSpeed);
  delay(1000);
  Stop();
  delay(500);
  Derecha(TurnSpeed);
  delay(1000);
  Stop();
  delay(500);
}
void GuideMovmnt(int ENG1, int ENG2){
 /* if ()
  if (motor1speed > M1_maksimum_speed ) motor1speed = M1_maksimum_speed;
  if (motor2speed > M2_maksimum_speed ) motor2speed = M2_maksimum_speed;
  if (motor1speed < 0) motor1speed = 0; 
  if (motor2speed < 0) motor2speed = 0; 
  motor1.setSpeed(motor1speed); 
  motor2.setSpeed(motor2speed);
  motor1.run(FORWARD); 
  motor2.run(FORWARD);*/
}

void TurnRobot(int Turn){
  
}

void setup(){
  InitializeEngine();
  CalibrateSensor();
  GuideMovmnt(0,0);
  TurnRobot(0);
}

// the loop function runs over and over again forever
void loop() {
  int position = qtrrc.readLine(sensorValues);
  int A0 = sensorValues[0], A1 = sensorValues[1], A2 = sensorValues[2], A3 = sensorValues[3], A4 = sensorValues[4], A5 = sensorValues[5]; 
  for (unsigned char i = 0; i < NUM_SENSORS; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
   int deviation = position - 2500;
  Serial.println(position);
  Serial.println(deviation);
 
  //if ( position >= 2400 /*(A0 <= 200 && A1 <= 200 && A4 <= 200 && A5 <= 200) && (A2 >= 400 && A3 >= 400)*/ ){
    if ( deviation > 0){
      Adelante(ForwardSpeed + 10, ForwardSpeed - 10);
    } else if ( deviation < 400){
      Adelante(ForwardSpeed - 10, ForwardSpeed + 10);
    }// else Adelante(ForwardSpeed, ForwardSpeed);   
     
  /*}*/ else if ( (A4 <= 200 && A5 <= 200) && (A2 >= 400 && A3 >= 400 && A0 >= 400 && A1 >= 400) ){
      Derecha(TurnSpeed);
      delay(1000);
      Adelante(ForwardSpeed, ForwardSpeed);
      delay(1000); 
          
  } else if ( (A0 <= 200 && A1 <= 200) && (A2 >= 400 && A3 >= 400 && A4 >= 400 && A5 >= 400) ){
      Izquierda(TurnSpeed);
      delay(1000);
      Adelante(ForwardSpeed, ForwardSpeed);
      delay(1000);    
  }
    
  else Stop();
 }
