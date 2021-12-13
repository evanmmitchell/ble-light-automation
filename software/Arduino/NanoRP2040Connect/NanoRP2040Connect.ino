#include "src/LSM6DSOX/src/LSM6DSOXSensor.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

#include "secrets.h"


#define TRAIN '1'
#define CLASSIFY '0'


LSM6DSOXSensor imu(&Wire, LSM6DSOX_I2C_ADD_L);
volatile bool imuInterrupt = false;

IPAddress server(SECRET_IP);
WiFiClient wifiClient;
PubSubClient mqttClient(server, 1883, wifiClient);

float startTime;
int counter;


void setup() {
  Serial.begin(9600);
//  while(!Serial);

  Serial1.begin(9600);

  Wire.begin();
  imu.begin();
  imu.Enable_X();
  imu.Enable_Significant_Motion_Detection();
  attachInterrupt(INT_IMU, imuInterruptHandler, RISING);

  mqttClient.setBufferSize(50000);

  startTime = millis();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from Wi-Fi");
    while (WiFi.begin(SECRET_SSID, SECRET_PASSWORD) != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(2500);
    }
    Serial.println("Connected to Wi-Fi");
  }

  if (!mqttClient.connected()) {
    Serial.println("Disconnected from server");
    while (!mqttClient.connect("NanoRP2040Connect")) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(2500);
    }
    Serial.println("Connected to server");
  }

  if (Serial1.available()) {
    String bleJson = "";
    char c = 0;
    while (c != ']') {
      if (Serial1.available()) {
        c = Serial1.read();
        bleJson += c;
      }
    }
    Serial.println(bleJson);
    mqttClient.publish("home-assistant/ble-light-automation", bleJson.c_str());

    if (counter == 0) {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (imuInterrupt && significantMotionDetected()) {
    Serial.println("Significant motion detected");
    digitalWrite(LED_BUILTIN, HIGH);
    counter = 3;
    Serial1.print(CLASSIFY);
    startTime = millis() + 10000;
    counter--;
    imuInterrupt = false;
  }

  if (millis() > startTime) {
    if (counter > 0) {
      Serial1.print(CLASSIFY);
      startTime += 10000;
      counter--;
    } else {
      Serial1.print(TRAIN);
      startTime += 30000;
    }
  }

   mqttClient.loop();
}

void imuInterruptHandler() {
  imuInterrupt = true;
}

unsigned int significantMotionDetected() {
  LSM6DSOX_Event_Status_t status;
  imu.Get_X_Event_Status(&status);
  return status.SignificantMotionStatus;
}
