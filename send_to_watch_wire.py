import time
import serial

LOG_FILE = "./sensor_log.txt"

# Read the log file and get the most recent line
with open(LOG_FILE, "r") as file:
    lines = file.readlines()
    latest_data = lines[-1].strip() if lines else None

WATCH_PORT = "COM14"  # TinyScreen+ port
try:
    with serial.Serial(WATCH_PORT, 9600, timeout=2) as watch:
        watch.write(latest_data.encode())
        print("Message sent")
except Exception as e:
    print("Error communicating with TinyWatch+:", e)

