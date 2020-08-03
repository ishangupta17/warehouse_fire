#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
BlynkTimer timer;
char auth[] = "ZGCLEzz0omPMQlKGqP1ZUvtegwdyjhdR"; //Auth code sent via Email
char ssid[] = "Redmi"; //Wifi name
char pass[] = "ishan1234";  //Wifi Password
int flag = 0;
int flag1 = 0;

#define DHTPIN 0          // Digital pin 3
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//temperature sensor code
void tempSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if(t>=21  )//for relay 1
  {
    digitalWrite(D4,HIGH);
    digitalWrite(D0,LOW);
    Serial.println("AC on");
  }
  else if(t<=21 )//for relay 2
  {
    digitalWrite(D0,HIGH);
    digitalWrite(D4,LOW);
    Serial.println("HEATER ON");
  }
  else  // for both relays
  {
    digitalWrite(D4,LOW);
    digitalWrite(D0,LOW);
    Serial.println("both off");
  }
 
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}
void temps() //alerts
{
  float u = dht.readHumidity();
  float e = dht.readTemperature();
  Blynk.notify(String("temperature:")+ e + String(" humidity: ") + u);
}


//Flame Sensor Code
void notifyOnFire() 
{
  int isButtonPressed = !digitalRead(D1);
  int iiButtonPressed = !digitalRead(D2);
  if ((isButtonPressed==1 || iiButtonPressed==1) && flag==0) {
    Blynk.tweet("Alert : Fire in the wareHouse  @xyzp11");
    Serial.println("Fire in the wareHouse");
    Blynk.notify("Alert : Fire in the wareHouse");
    
    flag=1;
  }
  else if (isButtonPressed==0 || iiButtonPressed==0)
 {
    flag=0;
  }
 
}


//gas sensor code
void gassensor()
{     
  int gas_avalue;
  gas_avalue = analogRead(A0);
  if (gas_avalue >250 && flag1==0)
  {
    Blynk.tweet("Alert : gas in the wareHouse  @xyzp11");
    Serial.println("gas in the wareHouse");
    Blynk.notify("Alert : gas in the wareHouse");
    flag1 = 1;
    delay(20000);
  }
  else
  {
    flag1 = 0;
  }
  
}

void setup()
{
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
dht.begin();
pinMode(D1,INPUT_PULLUP);//for flame sensor 1
pinMode(D2,INPUT_PULLUP);//for flame sensor 2
pinMode(A0,INPUT);//for mq2
pinMode(D4,OUTPUT);//for relay 1
pinMode(D0,OUTPUT);//for relay 2
timer.setInterval(1000L,notifyOnFire); 
timer.setInterval(1000L, tempSensor);
timer.setInterval(1000L,gassensor);

}
void loop()
{
  Blynk.run();
  timer.run();
  unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if(currentmillis-previousmillis >= (60UL*60UL*1000UL))     //for delay alerts
  {
  temps();
  previousmillis=currentmillis;
  }

}
