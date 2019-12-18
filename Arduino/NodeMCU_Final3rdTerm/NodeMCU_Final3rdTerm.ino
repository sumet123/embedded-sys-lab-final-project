
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
#define AP_NAME "ฺBlind"
#define AP_PASSWORD "1234567890a" //8 characters or more in length
#define APPID   "EmbeddedLabFinalProject"
#define KEY     "3MiqKL2EM8Oc7uj"
#define SECRET  "lsj313LfirAtY06n3iIIEjV31"
#define ALIAS   "NodeMCU1"
#define TargetWeb "HTML_web"
String lineTok ;
String data;

// do when data come from web
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
    //send data to stm32
    chat.print(s2);
   
    
}
// do when connecting successed
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
  LINE.setToken(lineTok);
  //read data from stm32 through UART
  while(chat.available()){
    char recieved = chat.read();
    data.concat(recieved); 
  }
  // send message to microgear
  if (microgear.connected())
    {
       microgear.loop();
       char msg[128];
       if(data != ""){
        data.toCharArray(msg,data.length()+1);
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
  // check distance and send alert to line
    if(data == "n"){
      Serial.println("Alert");
      LINE.notify("Alert");
    }
   // set data to null string
    data ="";
    delay(500);
}
