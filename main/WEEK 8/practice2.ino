#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

int redPin = 9;
int greenPin = 10;
int bluePin = 11;

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  setColor(0, 0, 0);

  while (!huskylens.begin(mySerial)) {
    Serial.println("Begin failed!");
    delay(500);
  }

  Serial.println("HuskyLens RGB LED Ready!");
}

void loop() {
  if (!huskylens.request()) {
    setColor(0, 0, 0);
    return;
  }

  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();

    if (result.command == COMMAND_RETURN_BLOCK) {
      switch (result.ID) {
        case 1: // Red
          setColor(255, 0, 0);
          break;

        case 2: // Blue
          setColor(0, 0, 255);
          break;

        case 3: // Green
          setColor(0, 255, 0);
          break;

        case 4: // Yellow = Red + Green
          setColor(255, 255, 0);
          break;

        case 5: // Purple = Red + Blue
          setColor(255, 0, 255);
          break;

        default:
          setColor(0, 0, 0);
          break;
      }
    }
  }
  else {
    setColor(0, 0, 0);
  }

  delay(100);
}