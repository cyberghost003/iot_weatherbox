#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <dht.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#define sns D0
#define led D5
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

dht DHT;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

char auth[] = "ebd2009253bc45be9359c3d341fa83c9";
char ssid[] = "Tech_D0013164";
char pass[] = "YFDRVJHK";

float T=0, P=0, p0=0, a=0, h=0;

void DHT11()
{
  h=DHT.humidity;
  delay(1000);
}

void bmpsns()
{
  sensors_event_t event;
  bmp.getEvent(&event);
  
  if (event.pressure)
  {
    P = event.pressure;
    bmp.getTemperature(&T);
    p0 = SENSORS_PRESSURE_SEALEVELHPA;
    a = bmp.pressureToAltitude(p0, P);
  }
}

/*void xprint()
{
  Serial.print(T);
}*/

void check()
{
  digitalWrite(led,LOW);
  int chk = DHT.read11(sns);
  if(chk == DHTLIB_OK)
  {
    Serial.print("DHT Sucess");
  }
  else
  {
    Serial.print("DHT Error");
    digitalWrite(led,HIGH);
    while(1);
  }

  if(bmp.begin())
  {
    Serial.print("BMP Sucess");
  }
  else
  {
    Serial.print("BMP Error");
    digitalWrite(led,HIGH);
    while(1);
  }
}

BLYNK_READ(V0) //Blynk app has something on V5
{
  Blynk.virtualWrite(V0, T); //sending to Blynk
}

BLYNK_READ(V1) //Blynk app has something on V5
{
  Blynk.virtualWrite(V1, h); //sending to Blynk
}

BLYNK_READ(V2) //Blynk app has something on V5
{
  Blynk.virtualWrite(V2, P); //sending to Blynk
}

BLYNK_READ(V3) //Blynk app has something on V5
{
  Blynk.virtualWrite(V3, a); //sending to Blynk
}

/*void printd()
{
  Serial.println(T);
  Serial.println(h);
  Serial.println(P);
  Serial.println(a);
}*/

void setup() 
{
	Serial.begin(115200);
  digitalWrite(led,HIGH);
  delay(3000);
  check();
  Blynk.begin(auth, ssid, pass);
}

void loop() 
{
  DHT11();
  bmpsns();
  Blynk.run();
  //printd();
  delay(1000);
}




