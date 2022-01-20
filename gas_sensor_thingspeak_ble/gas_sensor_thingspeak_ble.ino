#include "WiFi.h"
#include "ThingSpeak.h"
#include "HTTPClient.h"
#include "BluetoothSerial.h"
#include <EEPROM.h>

BluetoothSerial ESP_BT; //Object for Bluetooth
String buffer_in;
unsigned long previousMillis = 0; 
byte val;       
int addr = 0;
byte indS=0;
byte indP=0;
String stream;
byte len=0;
String temp;
String temp2;
unsigned int interval=30000;

#define CHANNEL_ID "put here the channel ID on Thingspeak"
#define CHANNEL_API_KEY "put here the associated API key on Thingspeak"

WiFiClient client;
HTTPClient http;

const char* serverName = "put here the URL of the server";

#define gasSensorPin 34
int sensorValue;
int wifiValue;

int nb;
int trg;
int hit;

boolean check_wifiUpdate(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    Serial.println("30 Seconds Over");
    return true;
  }

  else if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    interval=50000;
    buffer_in = ESP_BT.readStringUntil('\n'); //Read what we recevive 
    //Serial.println("Received:"); Serial.println(buffer_in);
    delay(20);
 

    if(buffer_in.charAt(0)=='S')
    {
      for(int i=0;i<buffer_in.length();i++)
      {
        val=(byte)(buffer_in.charAt(i));
        //Serial.println("val "+val);
      
        EEPROM.write(addr, val);
        //Serial.println(val);
        addr++;
      }
      EEPROM.write(addr, 10);
      addr++;
      EEPROM.commit();     
      ESP_BT.println("SSID Stored");
    }

    else if(buffer_in.charAt(0)=='P')
    {
      for(int i=0;i<buffer_in.length();i++)
      {
        val=(byte)(buffer_in.charAt(i));
        //Serial.println("val "+val);
        EEPROM.write(addr, val);
        //Serial.println(val);
        addr++;
      }
      EEPROM.write(addr, 10);
      EEPROM.commit();  
      ESP_BT.println("Password Stored"); 
      return true;
    }  
 
    return false;
  }
  else{
    return false;
  }
}


void setup(){
  EEPROM.begin(50);
  Serial.begin(9600); //Start Serial monitor in 9600
  Serial.println("Bluetooth Device is Ready to Pair");

  Serial.println("Waiting For Wifi Updates 30 seconds");
  ESP_BT.begin("ESP32_BLUETOOTH"); //Name of bluetooth device

  while(!check_wifiUpdate()==true){}

  Serial.println("The Stored Wifi credetial are : ");
  for(int i=0;i<50;i++){
    val=EEPROM.read(i);
    stream+=(char)val;
    if((val==10) && (indS==0)){
      indS=i;
    }
    else if(val==10 && indP==0){
      indP=i;
      break;
    }
  }

  temp=stream.substring(0,indS);
  temp=temp.substring(5,indS);
  temp2=stream.substring(indS+1,indP);
  temp2=temp2.substring(5,indP-indS);

  int i=temp.length();
  int j=temp2.length();
  char ssid[i];
  char pass[j]; 
  temp.toCharArray(ssid,i);
  temp2.toCharArray(pass,j);

  Serial.println("Stored SSID");
  Serial.println(ssid);
  Serial.println("Stored PASS");
  Serial.println(pass);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  if(WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("WiFi Failed");
    while(1) {
        delay(1000);
    }
  }
  else{
    Serial.print("Wifi Connected to ");
    Serial.println(ssid);

  }

  ESP_BT.end();

  Serial.println("Gas sensor warming up!");
  
  ThingSpeak.begin(client);

  pinMode(gasSensorPin,INPUT);
  
  delay(20000); //20sec

  //set initial state for trg, nb, hit
  nb=0;//number of times the sensor enters aware mode
  trg=0;//sensor is in non trigger mode, no gas/smoke detected
  hit=0;//number of times gas is detected in aware mode
  
}


void loop(){
  sensorValue=analogRead(gasSensorPin);
  Serial.print("Gas value: ");
  Serial.println(sensorValue);
  wifiValue = WiFi.RSSI();
  ThingSpeak.setField(1, sensorValue);
  ThingSpeak.setField(2, wifiValue);
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
  if(trg==0){
    if(sensorValue>450){
      trg=1;
      hit++;
      nb++;
      delay(1000);
    }
    else delay(6000);
  }

  else if(trg==1){
    if(nb<5){
      nb++;
      if(sensorValue>450){
        hit++;
      }
      delay(1000);
    }
    else {
      trg=0;
      if(hit>=2){
        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");
        //introduce on mail_address space the email address you want to send the warning message to
        int httpResponseCode = http.POST("{\"value1\":\"mail_address\"}"); 
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        http.end();
        delay(600000);
      }
      nb=0;
      hit=0;
    }
  }
}
