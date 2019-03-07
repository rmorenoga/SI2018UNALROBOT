  int countA = 0;
  int positionA = 0;
  bool move = false;
  int positionB = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(10, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  positionA = analogRead(A0);
  if (move == true && positionA >= 992){
    countA += 1;
    move = false;
  }else if(move == false && positionA <= 984){
    move = true;
  }
  Serial.print(positionA);
  Serial.print(" ");
  Serial.println(countA);
}
