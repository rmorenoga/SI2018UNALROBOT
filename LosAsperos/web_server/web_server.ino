/*
 WiFiEsp example: WebServer
 A simple web server that shows the value of the analog input 
 pins via a web page using an ESP8266 module.
 This sketch will print the IP address of your ESP8266 module (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to display the web page.
 The web page will be automatically refreshed each 20 seconds.
 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(13, 12); // RX, TX
#endif

char ssid[] = "CC_Family";            // your network SSID (name)
char pass[] = "daniel25";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received


WiFiEspServer server(80);


void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}


void loop()
{
  // listen for incoming clients
  WiFiEspClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    WiFiEspClient cliente;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();        
        Serial.write(c);                 
        if ('a' == c) {
          char server[] = "192.168.0.8";
          if (cliente.connect(server, 443)) {
            Serial.println("Connected to server");
            // Make a HTTP request
            cliente.println("GET /send-percepts?move=av HTTP/1.1");
            cliente.println("Host: 192.168.0.8");
            cliente.println("Connection: close");                        
          }          
        }
        
      }
    }
    // give the web browser time to receive the data
    delay(10);
    
    // close the connection:
    //cliente.stop();
    Serial.println("Client disconnected");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
