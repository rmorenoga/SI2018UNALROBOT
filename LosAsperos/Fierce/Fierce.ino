#include <SoftwareSerial.h>

#define TIMEOUT 5000 // mS
#define or0 7
#define or1 6
#define or2 5
#define or3 4

#define PF 11
#define PA 10
#define PD 9
#define PIZ 8

#define LED 3

SoftwareSerial mySerial(13, 12); // RX, TX
int button_state = 0;

boolean SendCommand(String cmd, String ack);
void configPins();

void setup()
{ 
 Serial.begin(115200);
 mySerial.begin(115200);
 SendCommand("AT","OK");
 SendCommand("AT+CWMODE=2","OK"); //Para que acceder al robot como AP
 //SendCommand("AT+CWMODE=1","OK"); // Para que el robot sea parte de una red local
 //SendCommand("AT+CWJAP=\"your_ssid\",\"your_pass\"","OK");
 SendCommand("AT+RST", "Ready");  ;
 SendCommand("AT+CIFSR", "OK");
 SendCommand("AT+CWLIF", "OK");
 SendCommand("AT+CIPMUX=1","OK");
 SendCommand("AT+CIPSERVER=1,80","OK");
 SendCommand("AT+CIPMODE?","OK");
 digitalWrite(LED,HIGH);
}
 
void loop(){   
 button_state = digitalRead(or0);
 
 if(button_state == HIGH){
    mySerial.println("AT+CIPSEND=0,23");
    mySerial.println("Button was pressed!");
    Serial.write("PD");
    delay(1000);
    //SendCommand("AT+CIPCLOSE=0","OK");
  }
  
 String IncomingString="";
 boolean StringReady = false;
 
 while (mySerial.available()){
   IncomingString=mySerial.readString();
   StringReady= true;
  }
 
  if (StringReady){
    Serial.println("Received String: " + IncomingString);
  
  if (IncomingString.indexOf("LED=ON") != -1) {
    digitalWrite(LED,HIGH);
   }
 
  if (IncomingString.indexOf("LED=OFF") != -1) {
    digitalWrite(LED,LOW);
   }

  if (IncomingString.indexOf("AV") != -1) {
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
   }
  if (IncomingString.indexOf("STOP") != -1) {
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    }
  }   
 
}

void configPins(){
 pinMode(or0,OUTPUT);
 pinMode(or1,OUTPUT);
 pinMode(or2,OUTPUT);
 pinMode(or3,OUTPUT);

 pinMode(LED, OUTPUT);
 
 pinMode(PD,INPUT);
 pinMode(PA,INPUT);
 pinMode(PIZ,INPUT);
 pinMode(PF,INPUT);
}
 
boolean SendCommand(String cmd, String ack){
  mySerial.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
 
boolean echoFind(String keyword){
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  if (mySerial.available()){
    char ch = mySerial.read();
    Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println();
       return true;
    }
   }
  }
 return false; // Timed out
}
