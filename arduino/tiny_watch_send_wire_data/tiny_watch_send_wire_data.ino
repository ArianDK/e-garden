#include <TinyScreen.h>
#include <Wire.h>

TinyScreen display = TinyScreen(TinyScreenPlus);

#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif

// Parsed values
String date = "", timeStr = "", temperature = "", tempStatus = "", light = "", lightStatus = "", moisture = "", moistureStatus = "";

// Screen control
int currentScreen = 0; // 0 = screen1, 1 = screen2, 2 = clock
bool dataAvailable = false;

// Clock screen state
unsigned long lastClockUpdate = 0;
int hours = 0, minutes = 0, seconds = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.begin();
  display.setBrightness(10);
  display.setFlip(true);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();

  display.setCursor(0, 10);
  display.print("Waiting for data...");

  lastClockUpdate = millis();
}

void loop() {
  // === Serial Input ===
  if (SerialMonitorInterface.available()) {
    String line = SerialMonitorInterface.readStringUntil('\n');
    parseData(line);
    dataAvailable = true;
    displayScreen(); // Immediately show screen 0 if it's selected
  }

  // === Button Input ===
  unsigned char buttons = display.getButtons();

  if (buttons & TSButtonUpperLeft) {
    currentScreen = 0;
    displayScreen();
    delay(300);
  }

  if (buttons & TSButtonLowerLeft) {
    currentScreen = 1;
    displayScreen();
    delay(300);
  }

  if (buttons & TSButtonLowerRight) {
    currentScreen = 2;
    displayScreen();
    delay(300);
  }

  // === Clock Update ===
  if (currentScreen == 2 && millis() - lastClockUpdate >= 1000) {
    lastClockUpdate += 1000;
    updateClock();
    displayScreen(); // Refresh only if on clock screen
  }
}

void displayScreen() {
  display.clearScreen();

  if (!dataAvailable && currentScreen != 2) {
    display.setCursor(0, 10);
    display.print("Waiting for data...");
    return;
  }

  if (currentScreen == 0) {
    // Status screen
    display.setCursor(0, 0);
    display.print("Date: " + date);

    display.setCursor(0, 10);
    display.print("Time: " + timeStr);

    display.setCursor(0, 20);
    display.print(tempStatus);

    display.setCursor(0, 30);
    display.print(lightStatus);

    display.setCursor(0, 40);
    display.print(moistureStatus);

  } else if (currentScreen == 1) {
    // Sensor values screen
    display.setCursor(0, 0);
    display.print("Time: " + timeStr);

    display.setCursor(0, 10);
    display.print("Date: " + date);

    display.setCursor(0, 20);
    display.print("Temp: " + temperature);

    display.setCursor(0, 30);
    display.print("Light: " + light);

    display.setCursor(0, 40);
    display.print("Moist: " + moisture);

  } else if (currentScreen == 2) {
    // Clock screen
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);

    display.setCursor(0, 20);
    display.print("Clock:");

    display.setCursor(0, 30);
    display.print(buffer);
  }
}

void updateClock() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
  }
  if (minutes >= 60) {
    minutes = 0;
    hours++;
  }
  if (hours >= 24) {
    hours = 0;
  }
}

void parseData(String line) {
  int index = 0;
  int startIndex = 0;
  String parts[8];

  while (index < 8) {
    int sepIndex = line.indexOf(" | ", startIndex);
    if (sepIndex == -1) {
      parts[index] = line.substring(startIndex);
      break;
    } else {
      parts[index] = line.substring(startIndex, sepIndex);
      startIndex = sepIndex + 3;
    }
    index++;
  }

  if (index >= 7) {
    date = parts[0];
    timeStr = parts[1];
    temperature = parts[2].startsWith("Temp: ") ? parts[2].substring(6) : parts[2];
    tempStatus = parts[3];
    light = parts[4].startsWith("Light: ") ? parts[4].substring(7) : parts[4];
    lightStatus = parts[5];
    moisture = parts[6].startsWith("Moisture: ") ? parts[6].substring(10) : parts[6];
    moistureStatus = parts[7];
  }
}
