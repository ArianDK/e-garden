import time
import serial

WATCH_PORT = "COM14"  # Your TinyScreen+ is on this port
try:
    with serial.Serial(WATCH_PORT, 9600, timeout=2) as watch:
        watch.write(f"This is a message".encode())
        print("Message sent")
except Exception as e:
    print("Error communicating with TinyWatch+:", e)

