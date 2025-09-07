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
  
  if (strcmp(topic, topic_relay) == 0) {
    Serial.println("recive_relay");
    if (message == "Mon" ){Mode = true;}else if(message == "Moff"){Mode = false;}
    if (message == "1on" ){state1 = HIGH;}else if(message == "1off"){state1 = LOW;}
    if (message == "2on" ){state2 = HIGH;}else if(message == "2off"){state2 = LOW;}
    if (message == "3on" ){state3 = HIGH;}else if(message == "3off"){state3 = LOW;}
    if (message == "4on" ){state4 = LOW;}else if(message == "4off"){state4 = HIGH;}
    if (message == "5on" ){state5 = LOW;}else if(message == "5off"){state5 = HIGH;}
    if (message == "6on" ){state6 = LOW;}else if(message == "6off"){state6 = HIGH;}
    if (message == "7on" ){state7 = LOW;}else if(message == "7off"){state7 = HIGH;}

    digitalWrite(r1,state1);
    digitalWrite(r2,state2);
    digitalWrite(r3,state3);
    digitalWrite(r4,state4);
    digitalWrite(r5,state5);
    digitalWrite(r6,state6);
    digitalWrite(r7,state7);

  }
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public MQTT broker connected");
      client.subscribe(topic_relay);
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
  }
}