#include <Arduino.h>
#define FR_ENC_A 35   //M3
#define FR_ENC_B 34    
#define FL_ENC_A 32    //M2
#define FL_ENC_B 33      
#define RR_ENC_A 26   //M1
#define RR_ENC_B 25
#define RL_ENC_A 14   //M4
#define RL_ENC_B 27

// Volatile variables for each motor
volatile long fr_pulses = 0;
volatile long fl_pulses = 0;
volatile long rr_pulses = 0;
volatile long rl_pulses = 0;

// Interrupt functions for each encoder
void IRAM_ATTR countFR() {
  if (digitalRead(FR_ENC_B)) fr_pulses++; else fr_pulses--;
}

void IRAM_ATTR countFL() {
  if (digitalRead(FL_ENC_B)) fl_pulses++; else fl_pulses--;
}

void IRAM_ATTR countRR() {
  if (digitalRead(RR_ENC_B)) rr_pulses++; else rr_pulses--;
}

void IRAM_ATTR countRL() {
  if (digitalRead(RL_ENC_B)) rl_pulses++; else rl_pulses--;
}

void setup() {
  Serial.begin(115200);

  // Set all pins as inputs
  pinMode(FR_ENC_A, INPUT_PULLUP); pinMode(FR_ENC_B, INPUT_PULLUP);
  pinMode(FL_ENC_A, INPUT_PULLUP); pinMode(FL_ENC_B, INPUT_PULLUP);
  pinMode(RR_ENC_A, INPUT_PULLUP); pinMode(RR_ENC_B, INPUT_PULLUP);
  pinMode(RL_ENC_A, INPUT_PULLUP); pinMode(RL_ENC_B, INPUT_PULLUP);

  // Attach interrupts to the 'A' pins
  attachInterrupt(digitalPinToInterrupt(FR_ENC_A), countFR, RISING);
  attachInterrupt(digitalPinToInterrupt(FL_ENC_A), countFL, RISING);
  attachInterrupt(digitalPinToInterrupt(RR_ENC_A), countRR, RISING);
  attachInterrupt(digitalPinToInterrupt(RL_ENC_A), countRL, RISING);
}

void loop() {
  // Print values in a readable format
  Serial.print("FR: "); Serial.print(fr_pulses);
  Serial.print(" | FL: "); Serial.print(fl_pulses);
  Serial.print(" | RR: "); Serial.print(rr_pulses);
  Serial.print(" | RL: "); Serial.println(rl_pulses);

  delay(200);
}