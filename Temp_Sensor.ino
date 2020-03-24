
#include "NXTIoT_dev.h"
#include "DHT.h" 
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN 2 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 


NXTIoT_dev  mysigfox;



const int boton=6;
const int sensorPin = A0;
const int Battery_Pin = A1;
const int soil_temp_PIN = 3;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(soil_temp_PIN);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() 
{
  Serial.begin(9600);
  dht.begin(); //Se inician los sensores
  sensorDS18B20.begin(); 
  pinMode(boton, INPUT);
}



void loop() 
{
  if (digitalRead(boton)==LOW)
  {
    leer_temperatura();
    delay(1000);
  } 
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
}
