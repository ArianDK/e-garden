import serial
import time
from datetime import datetime

# === Constants ===
# Temperature thresholds (°C)
TEMP_TOO_COLD = 15.0
TEMP_OPTIMAL_LOW = 15.0
TEMP_OPTIMAL_HIGH = 27.0
TEMP_TOO_HOT = 27.0

# Light thresholds (0–1023)
LIGHT_LOW = 300
LIGHT_HIGH = 800

# Moisture thresholds (0–1023, lower = wetter)
MOISTURE_WET = 400
MOISTURE_DRY = 700

# === Serial setup ===
arduino = serial.Serial('COM10', 9600)  # Replace with your correct COM port
time.sleep(2)  # Allow Arduino to reset

def read_sensor_data(sample_count=10):
    temp_readings = []
    light_readings = []
    moisture_readings = []

    while len(temp_readings) < sample_count:
        line = arduino.readline().decode().strip()
        try:
            parts = line.split(",")
            if len(parts) != 3:
                raise ValueError("Invalid format")

            temp = float(parts[0])
            light = int(parts[1])
            moisture = int(parts[2])

            temp_readings.append(temp)
            light_readings.append(light)
            moisture_readings.append(moisture)

            print(f"Read: Temp={temp:.2f}°C, Light={light}, Moisture={moisture}")
        except ValueError:
            print("Invalid data:", line)

        time.sleep(1)

    return temp_readings, light_readings, moisture_readings

def classify_temperature(temp):
    if temp < TEMP_TOO_COLD:
        return "Too cold"
    elif temp > TEMP_TOO_HOT:
        return "Too hot"
    else:
        return "Optimal temperature"

def classify_light(light):
    if light < LIGHT_LOW:
        return "Too dark"
    elif light > LIGHT_HIGH:
        return "Too much light"
    else:
        return "Plant is chilling"

def classify_moisture(moisture):
    if moisture < MOISTURE_WET:
        return "All good (wet)"
    elif moisture > MOISTURE_DRY:
        return "Needs water"
    else:
        return "Does not water"

def log_status(avg_temp, avg_light, avg_moisture, status_temp, status_light, status_moisture):
    date = datetime.now().strftime("%Y-%m-%d")
    time = datetime.now().strftime("%H:%M:%S")
    with open("./sensor_log.txt", "a") as f:
        f.write(
            f"{date} | {time} | "
            f"Temp: {avg_temp:.2f}C | {status_temp} | "
            f"Light: {avg_light:.1f} | {status_light} | "
            f"Moisture: {avg_moisture:.1f} | {status_moisture}\n"
        )
    print("Logged to file.")

# === Main loop ===
print("Starting TinyGarden monitor. Press Ctrl+C to stop.")

try:
    while True:
        temp_data, light_data, moisture_data = read_sensor_data()
        avg_temp = sum(temp_data) / len(temp_data)
        avg_light = sum(light_data) / len(light_data)
        avg_moisture = sum(moisture_data) / len(moisture_data)

        status_temp = classify_temperature(avg_temp)
        status_light = classify_light(avg_light)
        status_moisture = classify_moisture(avg_moisture)

        print(f"\nAverages → Temp: {avg_temp:.2f}°C, Light: {avg_light}, Moisture: {avg_moisture}")
        print(f"Status → Temp: {status_temp}, Light: {status_light}, Moisture: {status_moisture}")

        log_status(avg_temp, avg_light, avg_moisture, status_temp, status_light, status_moisture)

        print("Waiting 10 minutes...\n")
        time.sleep(600)

except KeyboardInterrupt:
    print("Monitoring stopped by user.")
finally:
    arduino.close()
