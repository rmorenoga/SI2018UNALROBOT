int IN1=5;
int IN2=6;
int IN3=10;
int IN4=11;
int value_of_speed = 60;
int delayOFF = 1000;
int delayON = 500;

void setup() {
  configPins();
}

void loop(){
  avanzar();
  delay(delayOFF);
  retroceder();
  delay(delayOFF);
  derecha();
  delay(delayOFF);
  izquierda();
  delay(delayOFF);
}

void configPins() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void avanzar(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, 0);
  analogWrite(IN2, value_of_speed);
  analogWrite(IN3, value_of_speed);
  analogWrite(IN4, 0);
  Serial.println("Avanzan ambos motores");
  Serial.print("\t");
  delay(delayON);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void retroceder(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, value_of_speed);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, value_of_speed);
  Serial.println("Avanzan ambos motores");
  Serial.print("\t");
  delay(delayON);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void derecha(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, 0);
  analogWrite(IN2, value_of_speed);
  analogWrite(IN3, 0);
  analogWrite(IN4, value_of_speed);
  Serial.println("Avanzan ambos motores");
  Serial.print("\t");
  delay(delayON);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void izquierda(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(IN1, value_of_speed);
  analogWrite(IN2, 0);
  analogWrite(IN3, value_of_speed);
  analogWrite(IN4, 0);
  Serial.println("Avanzan ambos motores");
  Serial.print("\t");
  delay(delayON);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}
