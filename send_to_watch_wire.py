import time
import serial

LOG_FILE = "./sensor_log.txt"

# Read the log file and get the most recent line
with open(LOG_FILE, "r") as file:
    lines = file.readlines()
    latest_data = lines[-1].strip() if lines else None

WATCH_PORT = "COM14"  # TinyScreen+ port

def format_for_watch(data_line):
    # Example format:
    # 2025-07-23 | 09:34:31 | Temp: 23.20C | Optimal temperature | Light: 521.7 | Plant is chilling | Moisture: 3542.5 | Needs water
    if not data_line:
        return ""

    parts = data_line.split(" | ")

    try:
        date = parts[0]
        time = parts[1]
        temp = parts[2].split(":")[1].strip()
        tempStatus = parts[3].strip()
        light = parts[4].split(":")[1].strip()
        lightStatus = parts[5].strip()
        moisture = parts[6].split(":")[1].strip()
        moistureStatus = parts[7].strip()

        return f"{date} | {time} | {temp} | {tempStatus} | {light} | {lightStatus} | {moisture} | {moistureStatus}"

    except Exception as e:
        print("Error formatting data:", e)
        return ""

# Format data for display
formatted_data = format_for_watch(latest_data)

# Send to watch
try:
    with serial.Serial(WATCH_PORT, 9600, timeout=2) as watch:
        watch.write((formatted_data + "\n").encode())
        print("Message sent:", formatted_data)
except Exception as e:
    print("Error communicating with TinyWatch+:", e)