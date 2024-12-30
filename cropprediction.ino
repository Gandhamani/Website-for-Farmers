#include <LiquidCrystal.h>
int rs=13,en=12,d4=8,d5=9,d6=10,d7=11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dht.h>  
#include<Timer.h>
Timer t;
#include <SoftwareSerial.h>
SoftwareSerial Serial1(6, 7);

#define heart 13                              

char *api_key="ZWHZTI6B1KG0MZ66";     // Enter your Write API key from ThingSpeak
static char postUrl[150];
void httpGet(String ip, String path, int port=80);


#define dht_dpin 2 
dht DHT;
char MOISTURE_buff[20],CO_buff[20];
int MOISTURE_Value;

int phsensor=A1;

int nValue;
int pValue;
int kValue;
int PhValue;
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

void Ph_Check();

void HUMIDITY_MEASUREMENT();
void setup() 
{
    Serial.begin(9600);
   
     
lcd.begin(16, 2);
    Serial.println("CROP MONITORING AND PREDICTION....");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CROP YIELD");
    lcd.setCursor(0,1);
    lcd.print(" PREDICTION...");
    delay(2000);
    Serial1.begin(9600);
   Serial.begin(9600);
   lcd.clear();
   lcd.print("WIFI Connecting");
   lcd.setCursor(0,1);
   lcd.print("Please wait....");
  
   
   Serial.println("Connecting Wifi....");
   connect_wifi("AT",1000);
   connect_wifi("AT+CWMODE=1",1000);
   connect_wifi("AT+CWQAP",1000);  
   connect_wifi("AT+RST",5000);
   connect_wifi("AT+CWJAP=\"Meghaa\",\"123456789\"",10000);
   Serial.println("Wifi Connected"); 
   lcd.clear();
   lcd.print("WIFI Connected.");
   pinMode(heart, OUTPUT);
   delay(2000);
   t.oscillate(heart, 1000, LOW);
   t.every(20000, send2server);

}

void loop()                    
{
  
  MOISTURE_MONITOR();
  delay(1000);
  HUMIDITY_MEASUREMENT();
//  Ph_Check();
   N_Check();
   P_Check();
   K_Check();
   lcd.clear();
  lcd.print("N:");
  lcd.print(nValue);
  lcd.setCursor(6, 0);
  lcd.print("P:");
  lcd.print(pValue);
  lcd.setCursor(0, 1);
   lcd.print("K:");
  lcd.print(kValue);
//   lcd.setCursor(6, 1);
//  lcd.print("Ph:");
//  lcd.print(PhValue);
  delay(1000);
 
  t.update();
      
}

char Serial_read(void)
{
      char ch;
      while(Serial.available() == 0);
      ch = Serial.read(); 
      return ch;
}



void HUMIDITY_MEASUREMENT()
{
    DHT.read11(dht_dpin);
    Serial.print("Humidity: ");
    Serial.print(DHT.humidity);   // printing Humidity on LCD
    Serial.print(" %");
    Serial.print("  ");
    
    delay(500);

    Serial.print("Temperature:");
    Serial.print(DHT.temperature);   // Printing temperature on LCD
    Serial.print(" C");
    Serial.println("  ");

//     dtostrf(temperature, 5, 3, tempStr);
//      dtostrf(temperature, 5, 3, humidStr);
    

  lcd.clear();
  lcd.print("RH:");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("TEMP:");
  lcd.print(DHT.temperature);
  delay(500);

 }
void MOISTURE_MONITOR()
{
  MOISTURE_Value=analogRead(A0);
  MOISTURE_Value=1024-MOISTURE_Value;
  sprintf(MOISTURE_buff,"MOISTURE:%d",MOISTURE_Value);
  Serial.println(MOISTURE_Value);
  lcd.clear();
  lcd.print(MOISTURE_buff);
  delay(1000);

  
  

}
//void Ph_Check(){
//  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
//  { 
//    buf[i]=analogRead(phsensor);
//    delay(10);
//  }
//  for(int i=0;i<9;i++)        //sort the analog from small to large
//  {
//    for(int j=i+1;j<10;j++)
//    {
//      if(buf[i]>buf[j])
//      {
//        temp=buf[i];
//        buf[i]=buf[j];
//        buf[j]=temp;
//      }
//    }
//  }
//  avgValue=0;
//  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
//    avgValue+=buf[i];
//  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
//  PhValue=3.5*phValue;  
//
//  Serial.print("PhValue:");  
//  Serial.print(PhValue);
//  Serial.println("");
//  
//  delay(1000);
//}
void N_Check(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(phsensor);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  nValue=3.5*phValue*35;  

  Serial.print("NValue:");  
  Serial.print(nValue);
  Serial.println("");
  
  delay(1000);
}

 void P_Check(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(phsensor);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  pValue=3.5*phValue*10;  

  Serial.print("PValue:");  
  Serial.print(pValue);
  Serial.println("");
  delay(1000);

    }
 
 void K_Check(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(phsensor);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  kValue=3.5*phValue*12;  

  Serial.print("kValue:");  
  Serial.print(kValue);
  Serial.println("");
  delay(1000);

    }

void send2server()
{
  char humidityStr[8];
  char tempStr[8];
  char moistureStr[8];
  
  char nValueStr[8];
  char pValueStr[8];
  char kValueStr[8];

  char PhValueStr[8];
  
  
  dtostrf(DHT.temperature, 5, 3, tempStr);
  dtostrf(DHT.humidity, 5, 3, humidityStr);
  dtostrf(MOISTURE_Value, 5, 3, moistureStr);
  
  dtostrf(pValue, 5, 3, pValueStr);
  dtostrf(nValue, 5, 3, nValueStr);
  dtostrf(kValue, 5, 3, kValueStr);

//  dtostrf(PhValue, 5, 3, PhValueStr);


  sprintf(postUrl, "update?api_key=%s&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s",api_key,humidityStr,moistureStr,tempStr,nValueStr,pValueStr,kValueStr);
  httpGet("api.thingspeak.com", postUrl, 80);
}

//GET https://api.thingspeak.com/update?api_key=SIWOYBX26OXQ1WMS&field1=0

void httpGet(String ip, String path, int port)
{
  int resp;
  String atHttpGetCmd = "GET /"+path+" HTTP/1.0\r\n\r\n";
  //AT+CIPSTART="TCP","192.168.20.200",80
  String atTcpPortConnectCmd = "AT+CIPSTART=\"TCP\",\""+ip+"\","+port+"";
  connect_wifi(atTcpPortConnectCmd,1000);
  int len = atHttpGetCmd.length();
  String atSendCmd = "AT+CIPSEND=";
  atSendCmd+=len;
  connect_wifi(atSendCmd,1000);
  connect_wifi(atHttpGetCmd,1000);
}

void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
//    lcd.clear();
//    lcd.print(cmd);
    Serial.println(cmd);
    Serial1.println(cmd); 
    while(Serial1.available())
    {
      if(Serial1.find("OK"))

      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  {
   Serial.println("OK");
   lcd.clear();
        lcd.setCursor(0,1);
      lcd.print("OK");
  }
  else
  {
   Serial.println("Error");
//         lcd.setCursor(0,1);
//      lcd.print("Error");
  }
}