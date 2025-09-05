#include <Arduino.h>
#include <QTRSensors.h>

// --- ตั้งค่าพินและค่าต่างๆ ---
#define NUM_SENSORS 11
#define TIMEOUT 2500 // us
#define LED_BUILTIN 2

QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

uint8_t sensorPins[NUM_SENSORS] = {13, 12, 14, 27, 26, 25, 33, 32, 35, 34, 39};


#define MOTOR_L_F 16 // พินสำหรับมอเตอร์ซ้ายหมุนไปข้างหน้า
#define MOTOR_L_B 17 // พินสำหรับมอเตอร์ซ้ายหมุนไปข้างหลัง
#define MOTOR_R_F 18 // พินสำหรับมอเตอร์ขวาหมุนไปข้างหน้า
#define MOTOR_R_B 19 // พินสำหรับมอเตอร์ขวาหมุนไปข้างหลัง

#define PWMA 15
#define PWMB 4

#define STB 23

int baseSpeed;
int motorSpeed;
int maxSpeed = 200;

// PID สำหรับทางตรง (เร็ว, นิ่ง)
float Kp_straight = 0.15;
float Ki_straight = 0.0001;
float Kd_straight = 2.5;
int speed_straight = 200;

// PID สำหรับทางโค้ง (คม, เกาะ)
float Kp_curve = 0.35;
float Ki_curve = 0.0003;
float Kd_curve = 2.0;
int speed_curve = 180;

// Adaptive PID
float Kp, Ki, Kd;
float P, I, D;
int error = 0;
int lastError = 0;
//int I_max = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(STB, OUTPUT);
  digitalWrite(STB, LOW);

  // ตั้งค่าพินมอเตอร์เป็น OUTPUT
  pinMode(MOTOR_L_F, OUTPUT);
  pinMode(MOTOR_L_B, OUTPUT);
  pinMode(MOTOR_R_F, OUTPUT);
  pinMode(MOTOR_R_B, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // กำหนดค่าเริ่มต้นให้กับ QTRSensors
  qtr.setTypeAnalog();
  qtr.setSensorPins(sensorPins, NUM_SENSORS);
  qtr.setTimeout(TIMEOUT);

  Cali();
}

void loop() {
  
  uint16_t position = qtr.readLineBlack(sensorValues);
  error = position - 5000;

  // --- ส่วนของการสลับค่า PID (Gain Scheduling) ---
  if (abs(error) < 250) {// ถ้ารถเริ่มเข้าโค้ง (error มาก) -> ใช้ค่าสำหรับ "ทางโค้ง"
      Kp = 0.1;
      Ki = 0;
      Kd = 1.0;
      baseSpeed = speed_straight;
//      I_max = 3000;
    }
    else if (abs(error) > 4000) {// ถ้ารถเริ่มเข้าโค้ง (error มาก) -> ใช้ค่าสำหรับ "ทางโค้ง"
      Kp = Kp_curve;
      Ki = Ki_curve;
      Kd = Kd_curve;
      baseSpeed = speed_curve;
//      I_max = 6000;
    
    } else {
      // ถ้ารถวิ่งตรงดี (error น้อย) -> ใช้ค่าสำหรับ "ทางตรง"
      Kp = Kp_straight;
      Ki = Ki_straight;
      Kd = Kd_straight;
      baseSpeed = speed_straight;
//      I_max = 3000;
    }

  // --- คำนวณ PID (ใช้ค่า Kp, Ki, Kd ที่ถูกสลับมาแล้ว) ---
  P = error;
  I = I + error;
  
//  if (I > I_max) I = I_max;
//  if (I < -I_max) I = -I_max;

  D = error - lastError;
  lastError = error;

  motorSpeed = Kp * P + Ki * I + Kd * D;

  // จำกัดค่า motorSpeed
  if (motorSpeed > maxSpeed) {motorSpeed = maxSpeed;}
  if (motorSpeed < -maxSpeed) {motorSpeed = -maxSpeed;}

  // --- ปรับความเร็วมอเตอร์ ---
  int rightMotorSpeed = baseSpeed + motorSpeed;
  int leftMotorSpeed = baseSpeed - motorSpeed;

  // จำกัดความเร็วไม่ให้เกินค่าสูงสุด
  if (rightMotorSpeed > maxSpeed) {rightMotorSpeed = maxSpeed;}
  if (leftMotorSpeed > maxSpeed) {leftMotorSpeed = maxSpeed;}
  if (rightMotorSpeed < -maxSpeed) {rightMotorSpeed = -maxSpeed;}
  if (leftMotorSpeed < -maxSpeed) {leftMotorSpeed = -maxSpeed;}

  // ส่งค่าความเร็วไปยังมอเตอร์
  setMotorSpeed(leftMotorSpeed, rightMotorSpeed);

  // (Optional) แสดงค่าต่างๆ ทาง Serial Monitor เพื่อช่วยในการ Debug และ Tuning
//  Serial.print("Position: ");
//  Serial.println(position);
//  Serial.print(" Error: ");
//  Serial.println(error);
//  delay(33);
//   Serial.print(" Correction: ");
//   Serial.println(motorSpeed);
  // Serial.print(" Left Speed: ");
  // Serial.print(leftMotorSpeed);
  // Serial.print(" Right Speed: ");
  // Serial.println(rightMotorSpeed);
}
