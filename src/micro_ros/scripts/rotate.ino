#include <Arduino.h>

#define RPWM1 23   // M1 
#define LPWM1 22     
#define RPWM2 18   // M2
#define LPWM2 19      
#define RPWM3  2   // M3
#define LPWM3 4
#define RPWM4  21   // M4
#define LPWM4 5

void setup() {
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);
  pinMode(RPWM3, OUTPUT);
  pinMode(LPWM3, OUTPUT);
  pinMode(RPWM4, OUTPUT);
  pinMode(LPWM4, OUTPUT);
}

void loop() {
  // --- Motor 1 Move ---
  analogWrite(RPWM1, 125);
  analogWrite(LPWM1, 0);
  delay(5000);              // Wait 5 seconds
  analogWrite(RPWM1, 0);    // Stop Motor 1

  // --- Motor 2 Move ---
  analogWrite(RPWM2, 125);
  analogWrite(LPWM2, 0);
  delay(5000);              // Wait 5 seconds
  analogWrite(RPWM2, 0);    // Stop Motor 2

  // --- Motor 3 Move ---
  analogWrite(RPWM3, 125);
  analogWrite(LPWM3, 0);
  delay(5000);              // Wait 5 seconds
  analogWrite(RPWM3, 0);    // Stop Motor 3

  // --- Motor 4 Move ---
  analogWrite(RPWM4, 125);
  analogWrite(LPWM4, 0);
  delay(5000);              // Wait 5 seconds
  analogWrite(RPWM4, 0);    // Stop Motor 4

  // Optional: Add a delay here if you want a pause before the cycle repeats
  delay(1000); 
}

