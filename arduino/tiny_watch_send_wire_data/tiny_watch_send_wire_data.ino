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
  display.setFlip(true);  // Flip the screen 180 degrees
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();

  display.setCursor(0, 10);
  display.print("Waiting for data...");

  if (!hasReceived && SerialMonitorInterface.available() > 0) {
    // Read the Full Line of Data
    String line = SerialMonitorInterface.readStringUntil('\n');

    // Clears the TinyScreen+ display before showing new content
    display.clearScreen();
  }
}

void loop() {
  //TSButtonLowerLeft is for lower left button
    String line = SerialMonitorInterface.readStringUntil('\n');
    int y = 0;
    int startIndex = 0;

    while(true) {
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

      if (sepIndex == -1) break;  // stop after last or if out of space
    }

  bool button_UL = display.getButtons (TSButtonUpperLeft); //get button status
  int buttonReleased = 1; //flag

  if (buttonReleased && button_UL) { //wait for button press buttonReleased = 0; //toggle flag so this if statement is not true anymore
      display.clearScreen();
      display.setCursor(0, 10);
      display.print("It's Working");
  }

  //if (!buttonReleased && !button_UL) { buttonReleased = 1;} //toggle flag (dont need right now)
}