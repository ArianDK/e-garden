// TMP36: A0
// Light sensor: A1
// Capacitive Soil Moisture Sensor v1.2: A2

const int tempPin = A1;
const int lightPin = A0;
const int moisturePin = A2;

void setup() {
  Serial.begin(9600); // Begin serial communication
}

void loop() {
  // === Temperature ===
  int tempReading = analogRead(tempPin);
  float voltage = tempReading * (5.0 / 4095.0); //1023.0 for arduino uno
  float temperatureC = ((voltage - 0.5) * 100.0)/2;

  // === Light Level ===
  int lightLevel = analogRead(lightPin);  // 0–1023 (higher = more light)

  // === Moisture Level ===
  int moistureLevel = analogRead(moisturePin);  // 0–1023 (lower = wetter)

  // === Send CSV-formatted output ===
  Serial.print(temperatureC, 2);     // Temperature in °C (2 decimal places)
  Serial.print(",");
  Serial.print(lightLevel);         // Light level raw
  Serial.print(",");
  Serial.println(moistureLevel);    // Moisture level raw

  delay(1000); // Delay 1 second before next reading
}
