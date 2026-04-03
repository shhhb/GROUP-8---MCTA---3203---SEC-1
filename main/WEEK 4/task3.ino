#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;
Servo gateServo;

const int greenLedPin = 5; 
const int servoPin = 6;    
const int redLedPin = 7;   

// Sensitivity (adjust if needed)
const int motionThreshold = 2000;

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  gateServo.attach(servoPin);
  gateServo.write(90);

  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
}

void openGate() {
  for (int pos = 90; pos <= 180; pos += 2) {
    gateServo.write(pos);
    delay(15);
  }
}

void closeGate() {
  for (int pos = 180; pos >= 90; pos -= 2) {
    gateServo.write(pos);
    delay(15);
  }
}

bool detectMovement() {
  int16_t ax, ay, az;
  int16_t prev_ax, prev_ay, prev_az;

  // Get initial reading
  mpu.getAcceleration(&prev_ax, &prev_ay, &prev_az);

  // Check movement over multiple samples
  for (int i = 0; i < 20; i++) {
    mpu.getAcceleration(&ax, &ay, &az);

    if (abs(ax - prev_ax) > motionThreshold ||
        abs(ay - prev_ay) > motionThreshold ||
        abs(az - prev_az) > motionThreshold) {
      return true;
    }

    prev_ax = ax;
    prev_ay = ay;
    prev_az = az;

    delay(50);
  }

  return false;
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == '1') {
      Serial.println("Checking movement... Move sensor!");

      if (detectMovement()) {
        Serial.println("Movement detected!");

        digitalWrite(greenLedPin, HIGH);
        digitalWrite(redLedPin, LOW);
        openGate();
      } else {
        Serial.println("No movement!");

        digitalWrite(redLedPin, HIGH);
        digitalWrite(greenLedPin, LOW);
      }
    }

    else if (cmd == '0') {
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      closeGate();
    }

    else if (cmd == 'X') {
      for (int i = 0; i < 3; i++) {
        digitalWrite(redLedPin, HIGH);
        delay(150);
        digitalWrite(redLedPin, LOW);
        delay(150);
      }
      digitalWrite(redLedPin, HIGH);
    }
  }
}