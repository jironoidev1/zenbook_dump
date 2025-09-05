import cv2
import numpy as np

import paho.mqtt.client as mqtt
import time
import json 

MQTT_BROKER = "broker.emqx.io"  # หรือ IP Address ของ Broker ของคุณ
MQTT_PORT = 1883
MQTT_TOPIC = "opencv/detections" # ชื่อหัวข้อที่จะส่งข้อมูลไป

MQTT_COOLDOWN_SECONDS = 2.0  # ส่งข้อความทุกๆ 2 วินาทีเป็นอย่างน้อย
last_sent_time = 0   

# --- ฟังก์ชันสำหรับเชื่อมต่อ MQTT ---
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("เชื่อมต่อ MQTT Broker สำเร็จ!")
    else:
        print(f"เชื่อมต่อไม่สำเร็จ, code {rc}")

# --- สร้าง MQTT Client ---
client = mqtt.Client()
client.on_connect = on_connect
client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_start() # เริ่มการทำงานของ client ใน background

def detect_green_color(frame):
    global last_sent_time
    """
    ตรวจจับสีเขียวในภาพและแยกระหว่างเขียวเข้มและเขียวอ่อน
    """
    # แปลงภาพจาก BGR เป็น HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # กำหนดช่วงสีเขียวอ่อน (Light Green)
    # H: 40-80, S: 40-150, V: 150-255
    light_green_lower = np.array([40, 40, 150])
    light_green_upper = np.array([80, 150, 255])
    
    # กำหนดช่วงสีเขียวเข้ม (Dark Green)
    # H: 40-80, S: 150-255, V: 40-150
    dark_green_lower = np.array([40, 150, 40])
    dark_green_upper = np.array([80, 255, 150])
    
    # สร้าง mask สำหรับแต่ละสี
    mask_light_green = cv2.inRange(hsv, light_green_lower, light_green_upper)
    mask_dark_green = cv2.inRange(hsv, dark_green_lower, dark_green_upper)
    
    # ใช้ morphological operations เพื่อลด noise
    kernel = np.ones((5, 5), np.uint8)
    mask_light_green = cv2.morphologyEx(mask_light_green, cv2.MORPH_OPEN, kernel)
    mask_light_green = cv2.morphologyEx(mask_light_green, cv2.MORPH_CLOSE, kernel)
    
    mask_dark_green = cv2.morphologyEx(mask_dark_green, cv2.MORPH_OPEN, kernel)
    mask_dark_green = cv2.morphologyEx(mask_dark_green, cv2.MORPH_CLOSE, kernel)
    
    # หา contours สำหรับเขียวอ่อน
    contours_light, _ = cv2.findContours(mask_light_green, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    # หา contours สำหรับเขียวเข้ม
    contours_dark, _ = cv2.findContours(mask_dark_green, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    ripe_detected = False      # ตัวแปรสถานะ "สุก"
    unripe_detected = False  # ตัวแปรสถานะ "ไม่สุก"

    ripe_count = 0
    unripe_count = 0

    # วาดกรอบและใส่ข้อความสำหรับเขียวอ่อน
    for contour in contours_light:
        area = cv2.contourArea(contour)
        if area > 1000:  # กรองเฉพาะพื้นที่ที่ใหญ่พอ
            x, y, w, h = cv2.boundingRect(contour)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(frame, "UnRibe", (x, y - 10), 
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
            unripe_detected = True
            unripe_count += 1 # <-- 2. เพิ่มค่านับจำนวน

    
    # วาดกรอบและใส่ข้อความสำหรับเขียวเข้ม
    for contour in contours_dark:
        area = cv2.contourArea(contour)
        if area > 1000:  # กรองเฉพาะพื้นที่ที่ใหญ่พอ
            x, y, w, h = cv2.boundingRect(contour)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 150, 0), 2)
            cv2.putText(frame, "Ribe", (x, y - 10), 
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 150, 0), 2)
            ripe_detected = True
            ripe_count += 1 # <-- 2. เพิ่มค่านับจำนวน
    
    message_to_send = None
    if ripe_detected:
        message_to_send = "สุก"
    elif unripe_detected:
        message_to_send = "ไม่สุก"

    if message_to_send:
        current_time = time.time()
        if (current_time - last_sent_time) > MQTT_COOLDOWN_SECONDS:
            payload = json.dumps({"ribe": ripe_count, "unribe": unripe_count})
            client.publish(MQTT_TOPIC, payload)
            print(f"ส่ง MQTT: {payload}")
            last_sent_time = current_time 
    
    # สร้างภาพผลลัพธ์ของ mask
    result_light = cv2.bitwise_and(frame, frame, mask=mask_light_green)
    result_dark = cv2.bitwise_and(frame, frame, mask=mask_dark_green)
    
    # รวม mask ทั้งสองเข้าด้วยกัน
    combined_mask = cv2.bitwise_or(mask_light_green, mask_dark_green)
    combined_result = cv2.bitwise_or(result_light, result_dark)
    
    return frame, mask_light_green, mask_dark_green, combined_result

def main():
    # เปิดกล้อง (0 คือกล้องเริ่มต้น)
    cap = cv2.VideoCapture(0)
    
    # ตั้งค่าขนาดหน้าต่าง (optional)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    
    print("กด 'q' เพื่อออกจากโปรแกรม")
    print("กด 's' เพื่อบันทึกภาพ")
    
    while True:
        # อ่านภาพจากกล้อง
        ret, frame = cap.read()
        
        if not ret:
            print("ไม่สามารถอ่านภาพจากกล้องได้")
            break
        
        # ตรวจจับสีเขียว
        detected_frame, mask_light, mask_dark, combined_result = detect_green_color(frame)
        
        # แสดงผลหลายหน้าต่าง
        cv2.imshow('Original with Detection', detected_frame)
        # cv2.imshow('Light Green Mask', mask_light)
        # cv2.imshow('Dark Green Mask', mask_dark)
        # cv2.imshow('Combined Green Result', combined_result)
        
        # รอการกดปุ่ม
        key = cv2.waitKey(1) & 0xFF
        
        # กด 'q' เพื่อออก
        if key == ord('q'):
            break
        
        # กด 's' เพื่อบันทึกภาพ
        elif key == ord('s'):
            cv2.imwrite('detected_green.jpg', detected_frame)
            cv2.imwrite('light_green_mask.jpg', mask_light)
            cv2.imwrite('dark_green_mask.jpg', mask_dark)
            cv2.imwrite('combined_result.jpg', combined_result)
            print("บันทึกภาพเรียบร้อยแล้ว!")
    
    # ปิดกล้องและหน้าต่างทั้งหมด
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()