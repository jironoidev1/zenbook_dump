#include <WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

#define LED_BUILTIN 2

#define swPin1 13
#define swPin2 14

const char* ssid = "Pak_For_Iot";
const char* password = "xit9^ohe";

const char* mqtt_broker = "broker.emqx.io";
const char* mqtt_username = "Pak";
const char* mqtt_password = "park1234";
const int mqtt_port = 1883;
const char* topic_saipan = "kaipum/saipan";

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

//wifi
WiFiClient espClient;
PubSubClient client(espClient);

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 16, 17, 18, 19);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  reconnect();

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(swPin1, INPUT_PULLDOWN);
  pinMode(swPin2, INPUT_PULLDOWN);

}

void loop() {
  // step one revolution  in one direction:
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}

