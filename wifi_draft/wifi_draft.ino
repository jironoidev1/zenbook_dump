#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_wpa2.h"

// WiFi และ MQTT Configuration
const char* ssid = "@SD";
const char* username = "ajima.n@sd.ac.th"; 
const char* password = "Ajima0852394403";
const char* mqtt_broker = "broker.emqx.io";
const char* mqtt_username = "Pak";
const char* mqtt_password = "park1234";
const int mqtt_port = 1883;
const char* topic_send = "mqtt/send";
const char* topic_recive = "mqtt/recive";

WiFiClient espClient;
PubSubClient client(espClient);

#define LED_BUILTIN 2

int num = 0;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  reconnect();

  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  if (!client.connected()) {
    digitalWrite(LED_BUILTIN, LOW);
    reconnect();
  }

  num += 1;

  client.publish(topic_send, "Hello");
  client.publish(topic_send, String(num).c_str());

  client.loop();
}
