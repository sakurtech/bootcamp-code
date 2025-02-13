#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Edspert_sht20.h>
#include <EdspertPubSub.h>
#include <LDR.h>
#include <ArduinoJson.h> 

//========= JSON Declaration ================//
StaticJsonDocument<200> doc;
String jsonString, jsonPayload;

//========= Connection Declaration ================//
// Wifi Setting
/* USERNAME DAN PASSWORD WIFI */
//char* ssid = "KahfiQiana";
//char* password = "qiaT56$%^#@";

char* ssid = "username";
char* password = "password";

// Broker SAYA SENDIRI / DOMAINESIA
String MQTTUser = "user";
String MQTTPass = "pass";

/* 
    MQTT Setting 
    menggunakan broker eqmx sendiri
    VPS Domainnesia
*/
char *mqttServer = "127.0.0.1";
int mqttPort = 1883;
String myClientID = "bootcamp";
//String Topic_1 = "stendysakur/data/temp";
//String Topic_2 = "stendysakur/data/hum";
//String Topic_3 = "stendysakur/data/inten";
//String Topic_4 = "stendysakur/data/soil";
//String Topic_5 = "stendysakur/data/distance";

String Topic = "stendysakur/data/sensor";
extern String callBackPayload;    // Variabel untuk menerima Payload dari callback
extern String callBackTopic;      // Variabel untuk menerima Topic dari callback

EdspertPubSub clientMQTT;

//========= OLED LCD Declaration ================//
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//========= SHT Declaration ================//
HardwareSerial serial(2);
Edspert_sht20 sht20(&serial, 9600, 16, 17);

//========= LDR Declaration ================//
#define pinLDR 35
LDR ldr(pinLDR);

//========= Soil Moisture Declaration ================//
#define pinSOIL 34

//========= Distance Sensor ================//
#define ECHOPIN 13
#define TRIGPIN 12

float temperature, humidity, intensitas;
float soilMoisturePercent;
float distance;