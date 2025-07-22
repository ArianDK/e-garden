import time
import serial

WATCH_PORT = "COM14"  # Your TinyScreen+ is on this port
LOG_FILE = "./sensor_log.txt"  # Make sure the path is correct

def get_last_line(file_path):
    """Read the last non-empty line from the log file."""
    with open(file_path, "r") as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]
        return lines[-1] if lines else None

def parse_log_line(line):
    """Extract avg temp, light, and moisture from log line."""
    try:
        parts = line.split(" - Avg Temp: ")[1].split(" | ")
        temp = parts[0].split("C")[0].strip()
        light = parts[1].split("(")[0].strip()
        moisture = parts[2].split("(")[0].strip()
        return temp, light, moisture
    except Exception as e:
        print("Failed to parse log line:", e)
        return None, None, None

def send_to_watch(temp, light, moisture):
    """Send formatted data over serial to TinyScreen+."""
    try:
        with serial.Serial(WATCH_PORT, 9600, timeout=2) as watch:
            #watch.write(f"T:{temp}\n".encode())
            
            #watch.write(f"L:{light}\n".encode())
            print("Starting sending")
            watch.write(f"This is a message".encode())
            #watch.write(f"M:{moisture}\n".encode())
            print("Data sent to TinyWatch+:\n"
                  f"T:{temp}\nL:{light}\nM:{moisture}\n")
    except Exception as e:
        print("Error communicating with TinyWatch+:", e)

if __name__ == "__main__":
    line = get_last_line(LOG_FILE)
    if line:
        temp, light, moisture = parse_log_line(line)
        if temp:
            send_to_watch(temp, light, moisture)
        else:
            print("Could not extract data.")
    else:
        print("Log file is empty or missing.")
