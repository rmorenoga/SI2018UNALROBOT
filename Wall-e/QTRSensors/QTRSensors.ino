int8_t values[] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  getValues();
  for (int i = 0; i<6;i++){
    Serial.print(values[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(50);
}

void getValues(){
    values[0] = analogRead(A0);
    values[1] = analogRead(A1);
    values[2] = analogRead(A2);
    values[3] = analogRead(A3);
    values[4] = analogRead(A4);
    values[5] = analogRead(A5);
}
