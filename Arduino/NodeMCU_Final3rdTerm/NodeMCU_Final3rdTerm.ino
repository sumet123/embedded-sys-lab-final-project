
#include <EEPROM.h>
#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>            
#include <ESP8266WebServer.h>   
#include <WiFiManager.h>
#include <TridentTD_LineNotify.h>
#include <MicroGear.h>


SoftwareSerial chat(D6,D7); //RX,TX
WiFiClient client;
MicroGear microgear(client);
#define AP_NAME "ABC"
#define AP_PASSWORD "123456780a" //8 characters or more in length
#define APPID   "EmbeddedLabFinalProject"
#define KEY     "3MiqKL2EM8Oc7uj"
#define SECRET  "lsj313LfirAtY06n3iIIEjV31"
#define ALIAS   "NodeMCU1"
#define TargetWeb "HTML_web"
String lineTok ;
String data;


void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0'; //set array to null
    Serial.println((char *)msg);
    
    String s = ((char *)msg);
    String s1 = s.substring(0,s.indexOf(","));
    String s2 = s.substring(s.indexOf(",")+1);
    Serial.println(s1);
    Serial.println(s2);
    lineTok = s1;
    chat.print(s2);
   
    
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() {
  // put your setup code here, to run once:
  microgear.on(MESSAGE,onMsghandler);
  microgear.on(CONNECTED,onConnected);
  
  Serial.begin(115200);
  chat.begin(115200);
  EEPROM.begin(1024);
  
  WiFiManager wifiManager;
  
  //WiFiManager.resetSettings();
  wifiManager.autoConnect(AP_NAME); 
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(250);
     Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  microgear.init(KEY,SECRET,ALIAS);
  microgear.connect(APPID);
}

void loop() {
  // put your main code here, to run repeatedly:
  LINE.setToken(lineTok);
  while(chat.available()){
    char recieved = chat.read();
    data.concat(recieved); 
    //Serial.println(chat.available());
  }
  
  if (microgear.connected())
    {
       microgear.loop();
       //Serial.println("connected");
       char msg[128];
       if(data != ""){
        data.toCharArray(msg,data.length()+1);
        //Serial.println(msg);
        microgear.chat(TargetWeb , msg);
        
       }
           
       
    }
   else 
   {
    Serial.print("connection lost, reconnect");
    microgear.connect(APPID);
    while(!microgear.connected()){
      microgear.connect(APPID);
      Serial.print(".");
      delay(200);
    }
    Serial.println("Connected!");
    
   }
  //Serial.println(chat.available());
    if(data == "n"){
      Serial.println("Alert");
      LINE.notify("Alert");
    }
    data ="";
    delay(500);
}
