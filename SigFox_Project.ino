
#include "NXTIoT_dev.h"
#include "DHT.h" 
#include <OneWire.h>
#include <DallasTemperature.h>
#include "LowPower.h"

#define DHTPIN 2 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 


NXTIoT_dev  mysigfox;



const int sensorPin = A0;
const int Battery_Pin = A1;
const int soil_temp_PIN = 3;
const unsigned int MINUTES = 10;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(soil_temp_PIN);
DallasTemperature sensorDS18B20(&oneWireObjeto);

//===========================FUNCIONES===================//
void leer_temperatura();
void Sleep_minutes(unsigned int minutes);
//======================================================//

void setup() 
{
  Serial.begin(9600);
  dht.begin(); //Se inician los sensores
  sensorDS18B20.begin(); 
  delay(1000);
  leer_temperatura(); //At the begging send sensors data 
}



void loop() 
{

  Sleep_minutes(MINUTES); //Sleep the Board for N minutes
  delay(1000); //After Wake up give some time for make steady the system 
  leer_temperatura();
}


/******************* Local Functions*******************/

void leer_temperatura()
{
   float Battery_AR    = 0;
   float soil_hum_AR   = 0;
   float soil_temp     = 0; 
   int soil_hum        = 0;
   int air_hum         = 0;
   int air_temp        = 0;
   int Battery_Lvl     = 0;

  Battery_AR =  analogRead(Battery_Pin);
  Battery_Lvl = map(Battery_AR, 0, 1023, 0, 100);
  
  sensorDS18B20.requestTemperatures();
  soil_temp = sensorDS18B20.getTempCByIndex(0);
  
  air_hum  = dht.readHumidity(); //se lee la humedad
  air_temp = dht.readTemperature(); // se lee la temperatura
 
  soil_hum_AR = analogRead(sensorPin);
  soil_hum = map(soil_hum_AR, 0, 1023, 100, 0);
  

  Serial.print("Humedad del suelo: "); 
  Serial.print(soil_hum); 
  Serial.println(" %");

  Serial.print("Temperatura del suelo: "); 
  Serial.print(soil_temp); 
  Serial.println(" Grados Cº ");
  
  Serial.print("Humedad del aire: "); 
  Serial.print(air_hum); 
  Serial.println(" %");
 
  Serial.print("Temperatura del aire: "); 
  Serial.print(air_temp); 
  Serial.println(" Grados Cº"); 

  Serial.print("Nivel de Batería: "); 
  Serial.print(Battery_Lvl); 
  Serial.println(" %"); 
  Serial.println(" "); 
  


  mysigfox.initpayload();

  mysigfox.addint(air_hum);
  mysigfox.addint(air_temp);
  mysigfox.addint(soil_hum);
  mysigfox.addfloat(soil_temp);
  mysigfox.addint(Battery_Lvl);
  mysigfox.sendmessage();

  Serial.println(" "); 
}

void Sleep_minutes(unsigned int minutes)
{
 static int i = 0; //variable that handles all the entire loop
 static  int k = 0; //variable that handles an exact minute loop
  
  for(i = 0; i < minutes; i++)
  {
    //Loop that generates a minutes by 4secs
    for(k = 0; k < 16; k++)
    {
       // Enter power down state for 4 s with ADC and BOD module disabled
       LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);  
    }
    Serial.print(i);
  }
}
