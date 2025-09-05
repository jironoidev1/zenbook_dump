// --- ฟังก์ชันควบคุมมอเตอร์ ---
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  // --- ควบคุมมอเตอร์ซ้าย ---
  if (leftSpeed >= 0) { // หมุนไปข้างหน้า
    digitalWrite(MOTOR_L_F, HIGH);
    digitalWrite(MOTOR_L_B, LOW);
    analogWrite(PWMA, leftSpeed);
  } else { // หมุนกลับหลัง
    digitalWrite(MOTOR_L_F, LOW);
    digitalWrite(MOTOR_L_B, HIGH);
    analogWrite(PWMA, abs(leftSpeed)); // ใช้ค่าสัมบูรณ์
  }

  // --- ควบคุมมอเตอร์ขวา ---
  if (rightSpeed >= 0) { // หมุนไปข้างหน้า
    digitalWrite(MOTOR_R_F, HIGH);
    digitalWrite(MOTOR_R_B, LOW);
    analogWrite(PWMB, rightSpeed);
  } else { // หมุนกลับหลัง
    digitalWrite(MOTOR_R_F, LOW);
    digitalWrite(MOTOR_R_B, HIGH);
    analogWrite(PWMB, abs(rightSpeed)); // ใช้ค่าสัมบูรณ์
  }
}
