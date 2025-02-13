#include <math.h>

#include "header.h"

//========= Relay Declaration ================//
#define RELAY 4

//========= BUZZER Declaration ================//
#define BUZZER 27

String TopicSub = "stendysakur/data/pump";
String TopicSubBuzz = "stendysakur/data/sound";

unsigned long previousMillis = 0;  // Waktu sejak terakhir kali melakukan sesuatu
const long interval = 3000;        // Interval yang diinginkan dalam milidetik (2 detik)

float fltCallBackPayload;
int soundCallBackPayload;

void setup() {
  Serial.begin(115200);
  splashScreenOled();
  sht20.begin();
  ldr.begin();
  initDistanceSensor();
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  clientMQTT.connect_to_AP(ssid,password);
  clientMQTT.init_to_broker(mqttServer,mqttPort);
  clientMQTT.connect_to_broker(myClientID, MQTTUser, MQTTPass);
  clientMQTT.mqtt_subscribe (TopicSub);
  clientMQTT.mqtt_subscribe (TopicSubBuzz);
}

void loop() {
  clientMQTT.rxMessFrBroker();
  if (callBackTopic == TopicSub) {
    fltCallBackPayload = callBackPayload.toFloat();
  } 

  if (callBackTopic == TopicSubBuzz) {
    soundCallBackPayload = callBackPayload.toInt();
  }

  /* print kondisi fltCallBackPayload */
  Serial.print ( "Default fltCallBackPayload: ");
  Serial.println(fltCallBackPayload);

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    // Read sensor
    temperature = sht20.readTemperature();
    humidity = sht20.readHumidity();
    intensitas = roundf (ldr.readIntensitas() * 100) / 100;
    soilMoisturePercent = readSoilMoisture();
    distance = roundf (readDistance() * 100) / 100;

    // Convert to JSoN
    doc["temp"] = temperature;
    doc["hum"] = humidity;
    doc["inten"] = intensitas;
    doc["soil"] = soilMoisturePercent;
    doc["dist"] = distance;
    
    // Serializasikan objek JSON ke dalam string JSON
    jsonPayload = "";    // Riset Payload jsonPayload
    serializeJson(doc, jsonPayload);

    clientMQTT.mqtt_publish(Topic,String(jsonPayload));

    displayMeassuring(temperature, humidity, intensitas, soilMoisturePercent, distance);
    display.clearDisplay();
    previousMillis = currentMillis;
  }
  else{
    // Action 
    /*
    if (distance > 50){
      digitalWrite(BUZZER,HIGH);
    }
    else{
      digitalWrite(BUZZER,LOW);
    }*/

    if ( distance > 21 ){
      digitalWrite(BUZZER,HIGH);
      digitalWrite (RELAY, LOW);
      fltCallBackPayload = 0;
      digitalWrite(BUZZER,LOW);
    }
    else{
      digitalWrite(BUZZER,LOW);
    }

    // Action Based on Soil Moisture
    // soilMoisturePercent < 40 ||
    // hanya percobaan saja, nti aslinya ditanya
    if( soilMoisturePercent < 40 || fltCallBackPayload == 1 ){
      digitalWrite(RELAY,HIGH);

      Serial.print ("Status dari node-red [high]: ");
      Serial.println ( fltCallBackPayload );
    }
    else{
      digitalWrite(RELAY,LOW);

      Serial.print ("Status dari node-red [low]: ");
      Serial.println ( fltCallBackPayload );
    }

    // buat suara
    if (soundCallBackPayload == 1) {
      digitalWrite ( BUZZER, HIGH );
    }else{
      digitalWrite ( BUZZER, LOW );
    }
  }
}