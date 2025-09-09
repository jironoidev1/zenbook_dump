void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Message: ");
  Serial.println(message);
  Serial.println("-----------------------");
  
  if (strcmp(topic, topic_saipan) == 0) {
    Serial.println("recive_saipan");
    if (message == "left"){
      while (digitalRead(swPin1) == LOW) {
        myStepper.step(stepsPerRevolution);
      }
      myStepper.step(0);
      }
    if (message == "right"){
      while (digitalRead(swPin2) == LOW) {
        myStepper.step(-stepsPerRevolution);
      }
      myStepper.step(0);
      }
  }
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public MQTT broker connected");
      client.subscribe(topic_saipan);
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
  }
}