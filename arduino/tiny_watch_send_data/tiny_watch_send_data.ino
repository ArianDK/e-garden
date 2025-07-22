#include <TinyScreen.h>
#include <Wire.h>

TinyScreen display = TinyScreen(TinyScreenPlus);

/* serial communication */
#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

bool hasReceived = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.begin();
  display.setBrightness(10);
  display.setFlip(false);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();

  display.setCursor(0, 10);
  display.print("Waiting for data...");
}

void loop() {
  if (!hasReceived && SerialMonitorInterface.available() > 0) {
    String line = SerialMonitorInterface.readStringUntil('\n');

    SerialMonitorInterface.print("I received: ");
    SerialMonitorInterface.println(line);

    display.clearScreen();

    int y = 0;
    int startIndex = 0;

    while (true) {
      int sepIndex = line.indexOf(" | ", startIndex);

      String part;
      if (sepIndex == -1) {
        part = line.substring(startIndex);  // last part
      } else {
        part = line.substring(startIndex, sepIndex);
        startIndex = sepIndex + 3;  // move past " | "
      }

      display.setCursor(0, y);
      display.print(part);
      y += 10;

      if (sepIndex == -1 || y > 50) break;  // stop after last or if out of space
    }

    hasReceived = true;
  }
}
