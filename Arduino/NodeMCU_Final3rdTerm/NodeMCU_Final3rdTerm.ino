
#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include <DNSServer.h>            
#include <ESP8266WebServer.h>   
#include <WiFiManager.h>
SoftwareSerial chat(D6,D7); //RX,TX
#define AP_NAME "ABC"
#define AP_PASSWORD "123456780a" //8 characters or more in length
String data;



void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  chat.begin(115200);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect(AP_NAME,AP_PASSWORD); 
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(250);
     Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(chat.available()){
    char recieved = chat.read();
    data+= recieved; 
    //Serial.print('1');
  }
  if(data != ""){
    Serial.println(data);
    data = "";
  }
  //Serial.println(chat.available());
  delay(500);
  
}
