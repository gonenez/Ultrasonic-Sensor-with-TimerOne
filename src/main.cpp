#include <Arduino.h>
#include <TimerOne.h>

#define trig 2 // Trigger pin
#define echo 3 // Echo pin

volatile unsigned long t_0 = 0;
volatile unsigned long t_1 = 0;
volatile bool data_available = false;

void handleEcho() {
  if (digitalRead(echo) == HIGH) {
    t_0 = micros(); // Start time
  } else {
    t_1 = micros(); // End time
    data_available = true; // data ready
  }
}

void triggerUltrasonic() {
  // Send a pulse to the trig pin
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Attach interrupt to echo pin 
  attachInterrupt(digitalPinToInterrupt(echo), handleEcho, CHANGE);

  // Set up the timer to trigger every 500ms
  Timer1.initialize(500000); // 500ms
  Timer1.attachInterrupt(triggerUltrasonic);
}

void loop() {
  if (data_available) {
    noInterrupts(); 
    data_available = false;
    unsigned long duration = t_1 - t_0;
    interrupts();

    // convert distance to cm
    float distance = (duration * 0.0343) / 2.0;
    Serial.println(String(distance) + " cm");
  }
}