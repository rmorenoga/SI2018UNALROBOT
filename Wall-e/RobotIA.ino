#include <SoftwareSerial.h>

SoftwareSerial modulo(3,2); // Pin 3 receptor, Pin 2 transmisor

int IN1 = 6;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int value_of_speed = 70;
int enA = 4;
int enB = 5;
bool moveA = false;
bool moveB = false;
int positionA = 0;
int positionB = 0;
int s0, s1, s2, s3, s4, s5;
int delayON = 5;
int delayOFF = 25;

void setup(){
  configPins();
  Serial.begin(9600);
  modulo.begin(9600);
  configWifi();
  for(int i = 0; i < 50; i++){
    Serial.print(digitalRead(enA));
    Serial.print("\t");
    Serial.println(digitalRead(enB));
    delay(50);
  }
}

void loop(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  String string = "";
  String message = "";
  char c = "";
  bool signalMessage = false;
  while(modulo.available() > 0){
    c = modulo.read();
    string += c;
    if(signalMessage == true && (String)c != " "){
      message += c;
    }else{
      signalMessage = false;
      if(message != ""){
        if(message == "Avanzar"){
          Serial.print("Avanzando\n");
          positionA = digitalRead(enA);
          positionB = digitalRead(enB);
          if(!positionA){
            moveA = true;
          }
          if(!positionB){
            moveB = true;
          }
          avanzar();
        }else if(message == "Retroceder"){
          Serial.print("Retrocediendo\n");
          positionA = digitalRead(enA);
          positionB = digitalRead(enB);
          if(!positionA){
            moveA = true;
          }
          if(!positionB){
            moveB = true;
          }
          retroceder();
        }else if(message == "Derecha"){
          Serial.print("Girando a la derecha\n");
          positionA = digitalRead(enA);
          positionB = digitalRead(enB);
          if(!positionA){
            moveA = true;
          }
          if(!positionB){
            moveB = true;
          }
          derecha();
        }else if(message == "Izquierda"){
          Serial.print("Girando a la izquierda\n");
          positionA = digitalRead(enA);
          positionB = digitalRead(enB);
          if(!positionA){
            moveA = true;
          }
          if(!positionB){
            moveB = true;
          }
          izquierda();
        }
        message += "\n";
        Serial.print(message);
        message = "";
      }
    }
    if((String)c == "/" && (String)string[string.length()-2] == " "){
      signalMessage = true;
    }
  }
  s0 = analogRead(A0);
  s1 = analogRead(A1);
  s2 = analogRead(A2);
  s3 = analogRead(A3);
  s4 = analogRead(A4);
  s5 = analogRead(A5);
  Serial.print(s0);
  Serial.print("\t");
  Serial.print(s1);
  Serial.print("\t");
  Serial.print(s2);
  Serial.print("\t");
  Serial.print(s3);
  Serial.print("\t");
  Serial.print(s4);
  Serial.print("\t");
  Serial.println(s5);
}

void configPins(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enA, INPUT);
  pinMode(enB, INPUT);
}

void configWifi(){
  String string = "";
  char c = "";
  delay(1000);
  modulo.print("AT+CIPMUX=1\r\n");
  delay(1000);
  modulo.print("AT+CIPSERVER=1,80\r\n");
  delay(1000);
  while(modulo.available() > 0){
    c = modulo.read();
    string += c;
  }
  string += "\n";
  Serial.print(string);
}

void avanzar(){
  int countA = 0;
  int countB = 0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  while(countA < 20 || countB < 50){
    positionA = digitalRead(enA);
    positionB = digitalRead(enB);
    if((moveA && positionA) || (!moveA && !positionA)){
      countA += 1;
      moveA = !moveA;
    }
    if((moveB && positionB) || (!moveB && !positionB)){
      countB += 1;
      moveB = !moveB;
    }
    if(countA > countB){
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, value_of_speed);
      analogWrite(IN4, 0);
      Serial.println("Avanza solo motor 2");
    }else if(countB > countA){
      analogWrite(IN1, 0);
      analogWrite(IN2, value_of_speed);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
      Serial.println("Avanza solo motor 1");
    }else if(countA == countB){
      analogWrite(IN1, 0);
      analogWrite(IN2, value_of_speed);
      analogWrite(IN3, value_of_speed);
      analogWrite(IN4, 0);
      Serial.println("Avanzan ambos motores"); 
    }
    Serial.print(countA);
    Serial.print("\t");
    Serial.println(countB);
    delay(delayON);
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
    delay(delayOFF);
  }
}

void retroceder(){
  int countA = 0;
  int countB = 0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  while(countA < 20 || countB < 50){
    positionA = digitalRead(enA);
    positionB = digitalRead(enB);
    if((moveA && positionA) || (!moveA && !positionA)){
      countA += 1;
      moveA = !moveA;
    }
    if((moveB && positionB) || (!moveB && !positionB)){
      countB += 1;
      moveB = !moveB;
    }
    if(countA > countB){
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, value_of_speed);
      Serial.println("Avanza solo motor 2");
    }else if(countB > countA){
      analogWrite(IN1, value_of_speed);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
      Serial.println("Avanza solo motor 1");
    }else if(countA == countB){
      analogWrite(IN1, value_of_speed);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, value_of_speed);
      Serial.println("Avanzan ambos motores"); 
    }
    Serial.print(countA);
    Serial.print("\t");
    Serial.println(countB);
    delay(delayON);
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
    delay(delayOFF);
  }
}

void derecha(){
  int countA = 0;
  int countB = 0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  while(countA < 20 || countB < 50){
    positionA = digitalRead(enA);
    positionB = digitalRead(enB);
    if((moveA && positionA) || (!moveA && !positionA)){
      countA += 1;
      moveA = !moveA;
    }
    if((moveB && positionB) || (!moveB && !positionB)){
      countB += 1;
      moveB = !moveB;
    }
    if(countA > countB){
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, value_of_speed);
      Serial.println("Avanza solo motor 2");
    }else if(countB > countA){
      analogWrite(IN1, 0);
      analogWrite(IN2, value_of_speed);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
      Serial.println("Avanza solo motor 1");
    }else if(countA == countB){
      analogWrite(IN1, 0);
      analogWrite(IN2, value_of_speed);
      analogWrite(IN3, 0);
      analogWrite(IN4, value_of_speed);
      Serial.println("Avanzan ambos motores"); 
    }
    Serial.print(countA);
    Serial.print("\t");
    Serial.println(countB);
    delay(delayON);
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
    delay(delayOFF);
  }
}

void izquierda(){
  int countA = 0;
  int countB = 0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  while(countA < 20 || countB < 50){
    positionA = digitalRead(enA);
    positionB = digitalRead(enB);
    if((moveA && positionA) || (!moveA && !positionA)){
      countA += 1;
      moveA = !moveA;
    }
    if((moveB && positionB) || (!moveB && !positionB)){
      countB += 1;
      moveB = !moveB;
    }
    if(countA > countB){
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, value_of_speed);
      analogWrite(IN4, 0);
      Serial.println("Avanza solo motor 2");
    }else if(countB > countA){
      analogWrite(IN1, value_of_speed);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
      Serial.println("Avanza solo motor 1");
    }else if(countA == countB){
      analogWrite(IN1, value_of_speed);
      analogWrite(IN2, 0);
      analogWrite(IN3, value_of_speed);
      analogWrite(IN4, 0);
      Serial.println("Avanzan ambos motores"); 
    }
    Serial.print(countA);
    Serial.print("\t");
    Serial.println(countB);
    delay(delayON);
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
    delay(delayOFF);
  }
}
