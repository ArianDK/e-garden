// Not working (work in progress)
#include <TinyScreen.h>

TinyScreen display = TinyScreen(TinyScreenPlus);

String temp = "--";
String light = "--";
String moisture = "--";

/* serial communication */
#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

void setup() {
  SerialMonitorInterface.begin(9600);
  display.begin();
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  display.clearScreen();
  display.setCursor(0, 10);
  display.print("Waiting for data...");
}

void loop() {
  if (SerialMonitorInterface.available()) {
    String line = SerialMonitorInterface.readStringUntil('\n');
    if (line.startsWith("T:")) {
      temp = line.substring(2);
    } else if (line.startsWith("L:")) {
      light = line.substring(2);
    } else if (line.startsWith("M:")) {
      moisture = line.substring(2);
      updateDisplay();
    }
  }
}

void updateDisplay() {
  display.clearScreen();
  display.setCursor(0, 10);
  display.print("Temperature: ");
  display.print(temp);
  display.setCursor(0, 25);
  display.print("Light Level: ");
  display.print(light);
  display.setCursor(0, 40);
  display.print("Soil Moisture: ");
  display.print(moisture);
}
