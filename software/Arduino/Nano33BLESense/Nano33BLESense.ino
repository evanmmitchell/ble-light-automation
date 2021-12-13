#include <ArduinoBLE.h>


void setup() {
  Serial.begin(9600);
//  while(!Serial);

  Serial1.begin(9600);

  BLE.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial1.available()) {
    digitalWrite(LED_BUILTIN, HIGH);
    char train = Serial1.read();
    String bleJson = "[";
    bleJson += train;
    bleJson += ",";

    BLE.scan();
    bleJson += "{";
    float startTime = millis();
    while (millis() < startTime + 3000) {
      BLEDevice peripheral = BLE.available();
      if (peripheral) {
        bleJson += "\"";
        bleJson += peripheral.address();
        bleJson += "\":";
        bleJson += peripheral.rssi();
        bleJson += ",";
      }
    }
    bleJson.setCharAt(bleJson.length() - 1, '}');
    BLE.stopScan();

    bleJson += "]";
    Serial1.print(bleJson);
    Serial.println(bleJson);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
