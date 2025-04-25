#include <Servo.h>
#include "SevSeg.h"
#include <IRremote.h>

SevSeg sevseg;
Servo myServo;

unsigned long previousMillis = 0;
int seconds = 0;

const int RECV_PIN = A1;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  myServo.attach(A0);

  byte numDigits = 4;
  byte digitPins[] = { 2, 3, 4, 5 };
  byte segmentPins[] = { 6, 7, 8, 9, 10, 11, 12, 13 };
  bool resistorsOnSegments = false;

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    seconds++;

    if (seconds >= 60) {
      seconds = 0;
    }
  }

  if (seconds % 10 == 0) {
    myServo.write(45);
  } else {
    myServo.write(160);
  }

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == 0xFF02FD) {
  Serial.println("IR button triggered action!");
  myServo.write(45);
  delay(500);
  myServo.write(160);
}
    irrecv.resume();
  }

  sevseg.setNumber(seconds, 2);
  sevseg.refreshDisplay();
}
