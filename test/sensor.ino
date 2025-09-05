void Cali(){
  // --- ขั้นตอนการ Calibrate ---
  Serial.println("Starting Calibration... Please move the robot over the line.");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // เปิดไฟ LED บนบอร์ดเพื่อแสดงว่ากำลัง Calibrate

  for (int i = 0; i < 300; i++) {
    qtr.calibrate();
    delay(10);
  }
  digitalWrite(LED_BUILTIN, LOW); // ปิดไฟ LED
  
  Serial.println("Calibration finished.");
  Serial.println("Calibrated minimum values:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("Calibrated maximum values:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(2000);
  digitalWrite(STB, HIGH);
}
