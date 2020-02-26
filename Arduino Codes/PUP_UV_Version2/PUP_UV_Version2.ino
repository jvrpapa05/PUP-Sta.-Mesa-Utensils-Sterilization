// PUP STA. Mesa - UV
// version 1.0 - Opening and Closing lid.
// version 2.0 - With timer for UV and PTC.
// version 2.1 - Fixed timer problem (Pause and Start)

#include "timer.h"
#include "timerManager.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool onUV = false;
bool onPTC = false;
Timer timer1;
Timer timer2;

const int relayUV = 4;
const int relayPTC = 5;

const int motorForward = 6;
const int motorBackward = 7;

const int proximitySensor = 11;
int proximityVal;

const int redLED = 12;
const int buzzer = 13;

const int closeSwitch = A0;
int closeVal;

const int openSwitch = A1;
int openVal;

void setup() {
  Serial.begin(9600);
  Serial.println("BEGIN");

  timer1.setInterval(3000); // <---- SET INTERVAL FOR UV HERE!
  timer1.setCallback(uvFunction);

  timer2.setInterval(1000); // <---- SET INTERVAL FOR UV HERE!
  timer2.setCallback(ptcFunction);

  TimerManager::instance().start();

  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("First Line");

  pinMode(relayUV, OUTPUT);
  pinMode(relayPTC, OUTPUT);

  pinMode(motorForward, OUTPUT);
  pinMode(motorBackward, OUTPUT);

  pinMode(proximitySensor, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(closeSwitch, INPUT_PULLUP);
  pinMode(openSwitch, INPUT_PULLUP);

  digitalWrite(relayUV, HIGH);
  digitalWrite(relayPTC, HIGH);
}


void loop() {
  TimerManager::instance().update();
  readSensor();
}

void readSensor() {
  proximityVal = digitalRead(proximitySensor);
  Serial.println(proximityVal);

  while (proximityVal == LOW) {
    proximityVal = digitalRead(proximitySensor);

    do {
      Serial.println("Opening");
      digitalWrite(motorForward, HIGH);
      openVal = digitalRead(openSwitch);
    } while (openVal == HIGH);
    digitalWrite(motorForward, LOW);

    Serial.println("Opened");

    do {
      Serial.println("Standby");
      proximityVal = digitalRead(proximitySensor);
    } while (proximityVal == LOW);

    delay(1000);

    do {
      Serial.println("Closing");
      digitalWrite(motorBackward, HIGH);
      closeVal = digitalRead(closeSwitch);
    } while (closeVal == HIGH);
    digitalWrite(motorBackward, LOW);

    Serial.println("Closed");

  }

}



void uvFunction() {
  if (onUV) {
    digitalWrite(relayUV, HIGH);
  } else {
    digitalWrite(relayUV, LOW);
  }

  onUV = !onUV;
}


void ptcFunction() {
  if (onPTC) {
    digitalWrite(relayPTC, HIGH);
  } else {
    digitalWrite(relayPTC, LOW);
  }

  onPTC = !onPTC;
}
