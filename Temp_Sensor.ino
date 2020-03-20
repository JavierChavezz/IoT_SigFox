
#include "NXTIoT_dev.h"
#include "DHT.h" 

#define DHTPIN 2 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 


NXTIoT_dev  mysigfox;

const int boton=6;
const int sensorPin = A0;

void setup() 
{
  Serial.begin(9600);
  dht.begin(); //Se inicia el sensor
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
  //int sensorVal=analogRead(sensorPin);
  //float voltaje=(sensorVal/1024.0)*5;
  float h = dht.readHumidity(); //se lee la humedad
  float t= dht.readTemperature(); // se lee la temperatura
  //Serial.print("Voltaje: ");
  //Serial.println(voltaje); 

  Serial.println("Humedad: "); 
  Serial.println(h); 
 
  Serial.print("Grados Cº: "); 
  Serial.println(t); 


  //float temp=((voltaje)*100);

   
  //float temp=(35);
  //Serial.println(temp);
  mysigfox.initpayload();
  mysigfox.addfloat(h);
  mysigfox.addfloat(t);
  mysigfox.sendmessage();
}
