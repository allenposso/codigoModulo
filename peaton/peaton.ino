#include "ThingSpeak.h"

#include <ESP8266wifi.h>


// Open Lanuza Thingspeak Ver 1.3
//Publica datos en Thingspeak
//www.openlanuza.com

#define sema_rojo 0
#define sema_amarillo 5
#define sema_verde 4

#define sensor_trigger 12
#define sensor_echo 13
#define alarma 2

int pinLM35 = 1;

char ssid[] = "DANYSAN";
char pass[] = "sandany1812";
unsigned long myChannelNumber = 870220;
const char * myWriteAPIKey = "B79DK7T5TF7MN3RL";

int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {
pinMode(sema_rojo,OUTPUT);
  pinMode(sema_amarillo,OUTPUT);
  pinMode(sema_verde,OUTPUT);
  pinMode(sensor_trigger, OUTPUT);
  pinMode(sensor_echo, INPUT);
  pinMode(alarma, OUTPUT);
 
  digitalWrite(sensor_trigger, LOW);
  digitalWrite(alarma, LOW);
  digitalWrite(sema_rojo, false);
  digitalWrite(sema_amarillo, false);
  digitalWrite(sema_verde, false);
 
//NO TOCAR
WiFi.begin(ssid, pass); // Inicia WiFi
ThingSpeak.begin(client); // Inicia ThingSpeak


Serial.begin(115200); // Velocidad puerto serie

Serial.println();
Serial.print("Conectando a ");
Serial.print(ssid);

while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}

Serial.println();
Serial.println("Conectado a WiFi");
Serial.print("Dirección IP: ");
Serial.println(WiFi.localIP());

}

void loop() {
 
digitalWrite(sema_verde, true);
  for(int i =0; i<3;i++){
  Serial.println("-------------------");
  logistica(true);
  delay(3000/3);
  }
  delay(2000);
  digitalWrite(sema_verde, false);

  digitalWrite(sema_amarillo, true);
  for(int i =0; i<3;i++){
  Serial.println("-------------------");
  logistica(true);
  delay(1000/3);
  }
 
  delay(1000);
  digitalWrite(sema_amarillo, false);
 
  digitalWrite(sema_rojo, true);
  for(int i =0; i<3;i++){
  Serial.println("-------------------");
  logistica(true);
  delay(3000/3);
  }
  delay(3000);
  digitalWrite(sema_rojo, false);



}

void logistica(boolean valid){
  if(readSensor(valid)){
   digitalWrite(alarma, HIGH);
   delay(100);
   digitalWrite(alarma, LOW);
   }
 
  }

 boolean readSensor(boolean valid){
  boolean var = false;
  if(valid){
    long t;
    long d;

    digitalWrite(sensor_trigger, HIGH);
    delayMicroseconds(50);
    digitalWrite(sensor_trigger, LOW);
 
    t = pulseIn(sensor_echo, HIGH);
    d = t/59;
    var = d<10?true:false;
    Serial.print("valor: ");
    Serial.print(var);
   
    ThingSpeak.setField(2, var);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

   
  }
  return var;
}
