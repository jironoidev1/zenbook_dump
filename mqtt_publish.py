import paho.mqtt.client as mqtt
import time
import json # เพื่อส่งข้อมูลในรูปแบบ JSON

# --- ตั้งค่า MQTT ---
MQTT_BROKER = "broker.emqx.io"  # หรือ IP Address ของ Broker ของคุณ
MQTT_PORT = 1883
MQTT_TOPIC = "opencv/detections" # ชื่อหัวข้อที่จะส่งข้อมูลไป

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

while True:
    payload = json.dumps("AU-9kuy")
    # ส่งข้อมูล!
    client.publish(MQTT_TOPIC, payload)
    print(f"ส่งข้อมูล MQTT: {payload}") # แสดงผลใน Terminal
    time.sleep(1)