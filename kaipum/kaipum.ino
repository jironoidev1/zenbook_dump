#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <BH1750FVI.h>

#define LED_BUILTIN 4

//DHT
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//WaterTemp
#define ONE_WIRE_BUS 33 //กำหนดขาที่จะเชื่อมต่อ Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//WaterLevel
#define WLpin 35
int val = 0;

// TDS/EC
#define TdsSensorPin 32
#define VREF 1 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;


const char* ssid = "Pak_For_Iot";
const char* password = "xit9^ohe";

const char* mqtt_broker = "broker.emqx.io";
const char* mqtt_username = "Pak";
const char* mqtt_password = "park1234";
const int mqtt_port = 1883;
const char* topic_relay = "kaipum/relay";
const char* topic_hum = "kaipum/Hum";
const char* topic_TDS = "kaipum/TDS";
const char* topic_EC = "kaipum/EC";
const char* topic_temp = "kaipum/temp";
const char* topic_lux = "kaipum/lux";

//relay
int state1 = LOW;
int state2 = LOW;
int state3 = LOW;
int state4 = LOW;
int state5 = LOW;
int state6 = LOW;
int state7 = LOW;

int r1 = 25;
int r2 = 26;
int r3 = 27;
int r4 = 16;
int r5 = 17;
int r6 = 18;
int r7 = 19;

int Mode = false;

//Millis
unsigned long previousSensorMillis = 0;
const long sensorInterval = 1000;

//WIFI
WiFiClient espClient;
PubSubClient client(espClient);

// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

void setup() {
  
  Serial.begin(115200, SERIAL_8N1);
  Serial2.begin(115200, SERIAL_8N1, 13, 14);
  pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  reconnect();
  
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
  pinMode(r6, OUTPUT);
  pinMode(r7, OUTPUT);

  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(r3, LOW);
  digitalWrite(r4, LOW);
  digitalWrite(r5, LOW);
  digitalWrite(r6, LOW);
  digitalWrite(r7, LOW);

  pinMode(TdsSensorPin, INPUT);
  pinMode(WLpin, INPUT);
  
  dht.begin();
  sensors.begin();
  LightSensor.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }

  unsigned long currentMillis = millis();

  TDScal();

  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    sensors.requestTemperatures();
    float Wtemp = sensors.getTempCByIndex(0);

    float EC_sensor = float(analogRead(TdsSensorPin))/1024.0;
    
    int val = analogRead(WLpin);

    uint16_t lux = LightSensor.GetLightIntensity();

    Serial.print("Humidity: ");
    Serial.println(h);
    client.publish(topic_hum, String(h).c_str());

    Serial.print("EC : ");
    Serial.println(EC_sensor);
    client.publish(topic_EC, String(EC_sensor).c_str());

    Serial.print("Temperature is: ");
    Serial.print(Wtemp);
    client.publish(topic_temp, String(Wtemp).c_str());
    Serial.println(" *C");

    Serial.print("Light: ");
    Serial.println(lux);
    client.publish(topic_lux, String(lux).c_str());

    if (Mode == true ){
      if(val < 4000){
        digitalWrite(r3, LOW);
      }
      if(val > 4000){
        digitalWrite(r3, HIGH);
      }
    }
  
  }

  client.loop();
  
}