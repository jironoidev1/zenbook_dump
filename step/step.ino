#include <Stepper.h>

const int stepsPerRevolution = 200;

#define RX_PIN 25
#define TX_PIN 26

#define swpin1 13
#define swpin2 14

String message = "";

Stepper myStepper(stepsPerRevolution, 16, 17, 18, 19);
void setup() {
  Serial.begin(115200); // สำหรับ Serial Monitor
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // สำหรับการสื่อสารกับ ESP32 ตัวส่ง

  pinMode(swpin1, INPUT_PULLUP);
  pinMode(swpin2, INPUT_PULLUP);
  myStepper.setSpeed(60);
}

void loop() {
  // ตรวจสอบว่ามีข้อมูลเข้ามาหรือไม่
  if (Serial2.available()) {
    // อ่านข้อมูลทั้งหมดที่เข้ามา
    message = Serial2.readStringUntil('\n');
    Serial.println(message);
    
    if(message == "left"){
      while (digitalRead(swpin1) == HIGH){
        myStepper.step(stepsPerRevolution);
      }
      // myStepper.step(0);
    }
    if(message == "right"){
      while (digitalRead(swpin2) == LOW){
        myStepper.step(-stepsPerRevolution);
      }
      // myStepper.step(0);
    }
  }
}