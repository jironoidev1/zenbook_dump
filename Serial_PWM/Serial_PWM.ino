String message = "";
int ADC = 13;
int speed = 0;
int swPin = 16;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ADC, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(swPin, INPUT_PULLDOWN);

  digitalWrite(26, HIGH);
  digitalWrite(27, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    message = Serial.readStringUntil('\n');
    speed = message.toInt();
  }

  if (digitalRead(swPin) == HIGH){analogWrite(ADC, speed); swOUT(50);}
  else {analogWrite(ADC, 0); swOUT(50);}
  
}
