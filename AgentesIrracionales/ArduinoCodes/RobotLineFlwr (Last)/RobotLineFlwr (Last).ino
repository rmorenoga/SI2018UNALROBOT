#include <QTRSensors.h> //Pololu QTR Sensor Library. First you must download and install QTRSensors library

#define KP 2                  // Ajuste de velocidad minimo respecto al error
#define KD 5                  // Ajuste de velocidad maximo respecto al error
#define KrflSpeed1 200        // Velocidad minima del motor 1
#define KrflSpeed2 200        // Velocidad minima del motor 2
#define HighSpeed1 250        // Velocidad maxima del motor 1
#define HighSpeed2 250        // Velocidad maxima del motor 2
#define NUM_SENSORS 6         // Numero de sensores usados
#define TIMEOUT 2500          // Espera 2500 ms para que las salidas de los sensores bajen
#define EMITTER_PIN 6         // EmitterPin es el pin digital de Arduino que controla si los LED QTR están encendidos o apagados. El emisor es controlado por pin digital 6.
#define DEBUG 0

QTRSensorsRC qtrrc((unsigned char[]) { 14, 15, 16, 17, 18, 19 } ,NUM_SENSORS, TIMEOUT, EMITTER_PIN); //Asigna los pines analogos en el arduino de A0 a A5
  
unsigned int sensorValues[NUM_SENSORS];     // Arreglo asignado a almacenar los valores de la señal de QTR
int IN1 = 4, IN2 = 5, IN3 = 3, IN4 = 2;     // Define los pines del motor
int ENALeft = 10, ENARight = 9;             // Define el pin de señal del motor (0 - 255)
int lastError = 0;

void CalibrateSensors() {                   // Calibracion del sensor QTR respecto al suelo
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 250; i++){
    qtrrc.calibrate(QTR_EMITTERS_ON);
    delay(20);
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  if (DEBUG) {
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++){
      Serial.print(qtrrc.calibratedMinimumOn[i]);
      Serial.print(' ');
    }
    Serial.println();
  
    for (int i = 0; i < NUM_SENSORS; i++){
      Serial.print(qtrrc.calibratedMaximumOn[i]);
      Serial.print(' ');
    }
    Serial.println();
    Serial.println();
}}

void InitializeEngine(){    // Inicializa los pines asignados al motor como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENALeft, OUTPUT);
  pinMode(ENARight, OUTPUT);
}
  
void Adelante(int FwlSignal, int FwrSignal, char MOVED1, char MOVED2, char MOVEI1, char MOVEI2){    // Pone en marcha los motores respecto a la instruccion enviada+
  
  digitalWrite(IN1, MOVEI1);
  digitalWrite(IN2, MOVEI2);
  analogWrite(ENALeft, FwlSignal);
  digitalWrite(IN3, MOVED1);
  digitalWrite(IN4, MOVED2);
  analogWrite(ENARight, FwrSignal);  
}

void setup(){
  delay(1500);
  CalibrateSensors();
  InitializeEngine();
  GuideMotors(0,0);
}
  

  
void loop(){
  unsigned int sensors[6];
  int position = qtrrc.readLine(sensors);
  int error = position - 2500;
  
  int ENA = KP * error + KD * (error - lastError);
  lastError = error;
  
  int Set_LftEngine = KrflSpeed1 + ENA;
  int Set_RgtEngine = KrflSpeed2 - ENA;
  
  GuideMotors(Set_LftEngine, Set_RgtEngine);
}
  
void GuideMotors(int ENA_L, int ENA_R){
  char MOVED1, MOVED2, MOVEI1, MOVEI2;
  if (ENA_L > HighSpeed1 ) {
    ENA_L = HighSpeed1;
    MOVED1 = LOW, MOVED2 = HIGH, MOVEI1 = LOW, MOVEI2 = HIGH; 
  }
  if (ENA_R > HighSpeed2 ) {
    ENA_R = HighSpeed2;
    MOVED1 = LOW, MOVED2 = HIGH, MOVEI1 = LOW, MOVEI2 = HIGH; 
  }
  if (ENA_L < 0) {
    ENA_L = HighSpeed1;
    MOVED1 = HIGH, MOVED2 = LOW, MOVEI1 = LOW, MOVEI2 = HIGH;  
  }
  if (ENA_R < 0) {
    ENA_R = HighSpeed1;
    MOVED1 = LOW, MOVED2 = HIGH, MOVEI1 = HIGH, MOVEI2 = LOW;  
  }
  Adelante(ENA_L, ENA_R, MOVED1, MOVED2, MOVEI1, MOVEI2);
}
