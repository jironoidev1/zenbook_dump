//เพิ่ม client.subscribe({topic}); เพื่อ sub

void reconnect() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public MQTT broker connected");
      client.subscribe(topic_recive);
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(message);

  if (strcmp(topic, topic_recive) == 0) {
    if(message == "hello"){
      Serial.println("wa");
    }
  }
}