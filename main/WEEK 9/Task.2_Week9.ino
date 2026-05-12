#include "BluetoothSerial.h"
#include <ESP32Servo.h>
#include "DHT.h"


#define DHTPIN 15       // DHT22 Data pin connected to D15
#define DHTTYPE DHT22
const int servoPin = 13; // Servo signal pin connected to D13

BluetoothSerial SerialBT;
Servo myServo;
DHT dht(DHTPIN, DHTTYPE);

char command;
unsigned long previousMillis = 0;
const long interval = 2000; // 2 second interval for DHT22

void setup() {
 
  Serial.begin(9600);
  
  
  SerialBT.begin("Group 8_MSI"); 
  
  
  dht.begin();
  
  // Setup Servo
  myServo.attach(servoPin);
  myServo.write(0); // Start at home position

  Serial.println("System Initialized.");
  Serial.println("Bluetooth: Group 8_MSI | Sensor: D15 | Servo: D13");
}

void loop() {
  // 1. BLUETOOTH COMMAND LISTENER (Instant Response)
  if (SerialBT.available()) {
    command = SerialBT.read();
    
  
    Serial.print("Command received: ");
    Serial.println(command);

    switch (command) {
      case 'A':
        myServo.write(0);
        SerialBT.println("Action: Servo moved to 0°");
        break;
      case 'B':
        myServo.write(90);
        SerialBT.println("Action: Servo moved to 90°");
        break;
      case 'C':
        myServo.write(180);
        SerialBT.println("Action: Servo moved to 180°");
        break;
    }
  }

  // 2. SENSOR MONITORING (Runs every 2 seconds)
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    
    if (isnan(h) || isnan(t)) {
      Serial.println("Sensor Error: Check wiring at D15!");
      SerialBT.println("Error: Sensor not responding.");
    } else {
      // Send to phone
      SerialBT.print("Temp: ");
      SerialBT.print(t);
      SerialBT.print("C | Hum: ");
      SerialBT.print(h);
      SerialBT.println("%");

      // Print to Laptop Serial Monitor
      Serial.print("Temp: ");
      Serial.print(t);
      Serial.print("C | Hum: ");
      Serial.print(h);
      Serial.println("%");
    }
  }
}