#include <TinyScreen.h>
#include <Wire.h>

TinyScreen display = TinyScreen(TinyScreenPlus);

#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif

// Parsed values
String date, timeStr, temperature, tempStatus, light, lightStatus, moisture, moistureStatus;

// Button screen toggle
bool showScreen1 = true;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.begin();
  display.setBrightness(10);
  display.setFlip(true);  // Flip display to correct orientation
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();

  display.setCursor(0, 10);
  display.print("Waiting for data...");
}

void loop() {
  // === Parse incoming serial line ===
  if (SerialMonitorInterface.available()) {
    String line = SerialMonitorInterface.readStringUntil('\n');

    parseData(line);
    displayScreen();
  }

  // === Button handling ===
  unsigned char buttons = display.getButtons();

  // Lower left button toggles to screen 2
  if (buttons & TSButtonLowerLeft) {
    showScreen1 = false;
    displayScreen();
    delay(300); // debounce
  }

  // Upper left button toggles to screen 1
  if (buttons & TSButtonUpperLeft) {
    showScreen1 = true;
    displayScreen();
    delay(300); // debounce
  }
}

void displayScreen() {
  display.clearScreen();

  if (showScreen1) {
    // Screen 1: Date | Time | Status messages
    display.setCursor(0, 0);
    display.print(date);

    display.setCursor(0, 10);
    display.print(timeStr);

    display.setCursor(0, 20);
    display.print(tempStatus);

    display.setCursor(0, 30);
    display.print(lightStatus);

    display.setCursor(0, 40);
    display.print(moistureStatus);
  } else {
    // Screen 2: Time | Date | Raw sensor readings
    display.setCursor(0, 0);
    display.print(date);

    display.setCursor(0, 10);
    display.print(timeStr);

    display.setCursor(0, 20);
    display.print("Temprature: " + temperature);

    display.setCursor(0, 30);
    display.print("Light: " + light);

    display.setCursor(0, 40);
    display.print("Moisture: " + moisture);
  }
}

void parseData(String line) {
  int index = 0;
  int startIndex = 0;
  String parts[8];

  // Split data using " | "
  while (index < 8) {
    int sepIndex = line.indexOf(" | ", startIndex);
    if (sepIndex == -1) {
      parts[index] = line.substring(startIndex);
      break;
    } else {
      parts[index] = line.substring(startIndex, sepIndex);
      startIndex = sepIndex + 3;
    }
    SerialMonitorInterface.println(parts[index]);
    index++;
  }

  // Assign parsed values, removing labels like "Temp:"
  date = parts[0];
  timeStr = parts[1];
  temperature = parts[2].startsWith("Temp: ") ? parts[2].substring(6) : parts[2];
  tempStatus = parts[3];
  light = parts[4].startsWith("Light: ") ? parts[4].substring(7) : parts[4];
  lightStatus = parts[5];
  moisture = parts[6].startsWith("Moisture: ") ? parts[6].substring(10) : parts[6];
  moistureStatus = parts[7];
}
