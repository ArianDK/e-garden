# E-Garden  
**A Smart Gardening Assistant to Reduce Food Insecurity**

E-Garden is a smart gardening solution designed to empower individuals and communities with limited access to fresh produce. By combining real-time environmental sensing, intuitive feedback, and accessible technology, E-Garden simplifies plant care and promotes healthier eating through gardening.

---

## Table of Contents
- [Background](#background)
- [Goals](#goals)
- [How It Works](#how-it-works)
- [Features](#features)
- [Hardware Setup](#hardware-setup)
- [Software Installation](#software-installation)
- [Data Flow](#data-flow)
- [Getting Started](#getting-started)
- [Support](#support)
- [License](#license)

---

## Background

Over **53.6 million** people in the U.S. live in low-income areas with limited access to fresh produce—defined as more than 0.5 miles (urban) or 10 miles (rural) from the nearest supermarket (USDA).

Food insecurity, the lack of reliable access to nutritious food, is often exacerbated by:
- Low income and busy lifestyles
- High cost or poor availability of fruits and vegetables
- A lack of familiarity or confidence in preparing healthy meals (CDC)

---

## Goals

E-Garden aims to:
- Encourage gardening as a way to improve food access
- Empower users with minimal gardening experience
- Promote sustainability and healthy eating
- Provide a simple, scalable system for monitoring plant health

---

## How It Works

E-Garden uses a set of sensors connected to an Arduino that continuously monitor plant health and send status updates to a lightweight wearable display, the **TinyWatch**.

### Prototype Components:
- **Moisture Sensor** – detects soil dryness  
- **Light Sensor** – measures sunlight exposure  
- **Temperature Sensor** – tracks ambient air temperature  
- **Arduino Nano ESP32** – collects sensor data and sends it to a computer  
- **TinyWatch** – displays care messages in real time via Bluetooth or Wi-Fi  

---

## Features

- **Real-Time Monitoring**: Sensor data is updated every 10 minutes
- **Clear Plant Status**: Simple care suggestions displayed on TinyWatch
- **User-Friendly**: Quick tips make plant care easy for beginners
- **Sustainable**: Encourages self-grown produce with minimal maintenance
- **Scalable**: Monitor a single plant or expand to multiple garden beds
- **No Batteries Required**: Fully powered through USB

---

## Hardware Setup

**Required Components:**
- Arduino Nano ESP32
- TMP36 Analog Temperature Sensor
- Photoresistor (light sensor)
- Capacitive Soil Moisture Sensor v1.2
- Resistors and jumper wires
- Breadboard
- TinyWatch or compatible Bluetooth/Wi-Fi display

**Sensor Pin Mapping:**
- Temperature Sensor → `A0`  
- Light Sensor → `A1`  
- Moisture Sensor → `A2`  

---

## Software Installation

### Prerequisites:
- [Python 3.x](https://www.python.org/downloads/)
- [Arduino IDE](https://www.arduino.cc/en/software)
- Bluetooth or Wi-Fi enabled TinyWatch device

### Python Libraries:
```bash
pip install pyserial
```

Clone the repository:
```bash
git clone https://github.com/Arian-DK/e-garden.git
cd e-garden
```

Upload the Arduino sketch located in `/arduino/e_garden_sensor_reader.ino` to your Nano ESP32.

---

## Data Flow

```plaintext
[ Sensors ]
   ↓
[ Arduino Nano ESP32 ]
   ↓ (Serial every 10 min)
[ Computer (Python Script) ]
   ↓ (Writes to text file)
[ Text file ]
   ↓ (Bluetooth/Wi-Fi)
[ TinyWatch ]
```

---

## Getting Started

1. **Connect Sensors** to the Arduino according to the wiring diagram.
2. **Upload Arduino Code** using Arduino IDE.
3. **Run Python Script** on your computer to start reading and logging sensor data.
4. **Pair TinyWatch** via Bluetooth or Wi-Fi to begin receiving updates.
5. Watch your plants thrive with data-driven guidance.

---

## Support

If you encounter issues or have suggestions, please [open an issue](https://github.com/Arian-DK/e-garden/issues) or email the team.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
