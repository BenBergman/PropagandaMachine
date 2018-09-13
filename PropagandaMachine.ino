#include <Bounce2.h>

/*
 Keyboard test

 For the Arduino Leonardo, Micro or Due

 Reads a byte from the serial port, sends a keystroke back.
 The sent keystroke is one higher than what's received, e.g.
 if you send a, you get b, send A you get B, and so forth.

 The circuit:
 * none

 created 21 Oct 2011
 modified 27 Mar 2012
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/KeyboardSerial
 */

#include <Keyboard.h>

const int volPin = 0;
const int triggerPin = 8;

Bounce debouncer = Bounce();

void setup() {
  // open the serial port:
  Serial.begin(9600);
  // initialize control over the keyboard:
  //delay(4000);
  Keyboard.begin();
  pinMode(volPin, INPUT);

  pinMode(triggerPin, INPUT);
  digitalWrite(triggerPin, HIGH);
  debouncer.attach(triggerPin);
  debouncer.interval(5);
}

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

int prevTriggerState = HIGH;
int prevMicState = LOW;

void loop() {
  int reading = digitalRead(volPin);

  debouncer.update();

  int triggerState = debouncer.read();

  if (triggerState == LOW && prevTriggerState == HIGH) {
    Keyboard.press(' ');
    Serial.println("Pressing ' ' (trigger)");
    prevTriggerState = LOW;
  } else if (triggerState == HIGH && prevTriggerState == LOW) {
    Serial.println("trigger input stopped. Releasing all");
    Keyboard.releaseAll();
    prevTriggerState = HIGH;
  }

  if (reading == HIGH) {
    Serial.println("Mic input detected");
    lastDebounceTime = millis();
    prevMicState = HIGH;
  } else if (reading == LOW && prevMicState == HIGH) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      Serial.println("Mic input stopped. Releasing all");
      Keyboard.releaseAll();
      prevMicState = LOW;
      prevTriggerState = HIGH;
    }
  }
}

