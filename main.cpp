#include <Servo.h>;
#include "SevSeg.h"
SevSeg sevseg;
Servo myServo;

// Variables for timer
unsigned long previousMillis = 0;  // Last time the display was updated
unsigned long currentMillis = 0;   // Current time in milliseconds
int seconds = 0;                   // Timer seconds (to display on the 7-segment)

void setup() {
  // 7-segment display configuration
  myServo.attach(A0);
  byte numDigits = 4;
  byte digitPins[] = { 2, 3, 4, 5 };                                                     // Pins for each digit (adjust to your wiring)
  byte segmentPins[] = { 6, 7, 8, 9, 10, 11, 12, 13 };                                   // Pins for the segments (A to G)
  bool resistorsOnSegments = 0;                                                          // Set to 1 if resistors are on segments
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);  // Common cathode or anode
  sevseg.setBrightness(90);                                                              // Set display brightness (0 to 100)
}

void loop() {
  currentMillis = millis();  // Get the current time in milliseconds

  // If more than 1000 milliseconds have passed, update the timer
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;  // Save the last time update
    seconds++;                       // Increment seconds

    // Reset seconds after 60 to simulate minutes (optional)
    if (seconds >= 60) {
      seconds = 0;
    }
  }
  if (seconds % 10 == 0){
    myServo.write(45);
  }
  else {
    myServo.write(160);
  }
  
  // Display the elapsed seconds on the 7-segment display
  sevseg.setNumber(seconds, 2);  // Display seconds, 2 digits for `00` to `59`
  sevseg.refreshDisplay();       // Refresh display to update the numbers
}
