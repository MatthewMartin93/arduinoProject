#include "SevSeg.h"

// Instantiate a SevSeg object to control the 7-segment display
SevSeg sevseg;

// Pin definitions
const uint8_t IR_PIN = A2;  // The IR sensor is connected to analog pin A2

// Display number, initialized to 0
unsigned int displayNumber = 0;
unsigned int lastDisplay = 65535; // Start with a high number to ensure display updates on first loop

void setup() {
  // Setup serial communication for debugging (optional)
  Serial.begin(9600);

  // Pin assignments for digits and segments
  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5}; // Digit pins (one per digit of 7-segment display)
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // Segment pins A to G, plus DP if needed

  bool resistorsOnSegments = false; // Set to true if resistors are on segment pins
  bool updateWithDelays = false;
  bool leadingZeros = false; // Don't show leading zeros
  bool disableDecPoint = true; // Disable decimal point

  // Set the display type (COMMON_CATHODE or COMMON_ANODE based on your display)
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays,
               leadingZeros, disableDecPoint);
  
  sevseg.setBrightness(90); // Set display brightness (0-100)
  
  // Configure the IR sensor pin
  pinMode(IR_PIN, INPUT);

  // Test the segments
  testSegments();
}

void loop() {
  // Check IR sensor input
  checkIR();

  // Display the number only if it's different from the last displayed number
  if (displayNumber != lastDisplay) {
    sevseg.setNumber(displayNumber);
    lastDisplay = displayNumber;
  }

  // Update the display
  sevseg.refreshDisplay();
}

// Function to check the IR sensor and increment the display number when triggered
void checkIR() {
  static uint32_t lastTime = 0;  // Stores the last time the IR sensor was checked
  static uint8_t lastIR = digitalRead(IR_PIN);  // Stores the last state of the IR sensor
  uint32_t now = millis();  // Get current time

  if (now - lastTime >= 20) {  // Check every 20ms
    lastTime = now;

    uint8_t currentIR = digitalRead(IR_PIN);  // Read the current state of the IR sensor

    // If the state of the IR sensor has changed
    if (currentIR != lastIR) {
      lastIR = currentIR;

      // If the sensor is triggered (HIGH), increase the display number by 50
      if (currentIR == HIGH) {
        displayNumber += 50;

        // Ensure the number doesn't exceed 9999
        if (displayNumber > 9999) {
          displayNumber = 9999;
        }
      }
    }
  }
}

// Function to manually test each segment of the 7-segment display
void testSegments() {
  Serial.println("Testing segments...");

  // Test each segment individually: A to G
  for (int i = 0; i < 7; i++) {
    digitalWrite(6 + i, HIGH);  // Turns on each segment one by one
    delay(500);  // Wait for 0.5 second
    digitalWrite(6 + i, LOW);   // Turns the segment off
    Serial.print("Segment ");
    Serial.println(i + 1);
  }
}
