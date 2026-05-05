#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

int ledPin = 13;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  pinMode(ledPin, OUTPUT);

  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed! Check wiring and Protocol (Serial 9600)"));
    delay(500);
  }

  Serial.println("HuskyLens Ready!");
}

void loop() {
  if (!huskylens.request()) {
    digitalWrite(ledPin, LOW);
    return;
  }

  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();

    Serial.print("Object ");
    Serial.print(result.ID);
    Serial.println(" detected!");

    if (result.ID == 1) {
      digitalWrite(ledPin, HIGH);
    }
    else {
      digitalWrite(ledPin, LOW);
    }
  }
  else {
    Serial.println("Nothing detected");
    digitalWrite(ledPin, LOW);
  }

  delay(200);
}