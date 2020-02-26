// PUP STA. Mesa - UV
// version 1.0 - Opening and Closing lid.
// version 2.0 - With timer for UV and PTC.
// version 2.1 - Fixed timer problem (Pause and Start)
// version 4.0 - Request remove of timer, always ON if and only if no presence of object in IR Sensor. Prevent to false alarm.

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

  timer1.setInterval(5000); // <---- SET INTERVAL FOR UV HERE!
  timer1.setCallback(uvFunction);

  timer2.setInterval(10000); // <---- SET INTERVAL FOR PTC HERE!
  timer2.setCallback(ptcFunction);

  TimerManager::instance().start();

  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EATING UTENSILS");
  lcd.setCursor(0, 1);
  lcd.print("STERILIZATION USING");
  lcd.setCursor(0, 2);
  lcd.print("UV WITH PTC HEATER.");

  pinMode(relayUV, OUTPUT);
  pinMode(relayPTC, OUTPUT);

  pinMode(motorForward, OUTPUT);
  pinMode(motorBackward, OUTPUT);

  pinMode(proximitySensor, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(closeSwitch, INPUT_PULLUP);
  pinMode(openSwitch, INPUT_PULLUP);

  digitalWrite(relayUV, LOW);
  digitalWrite(relayPTC, LOW);
}


void loop() {
  //TimerManager::instance().update();
  readSensor();

  Serial.print(onPTC);
  Serial.println(onUV);
}

void readSensor() {
  proximityVal = digitalRead(proximitySensor);
  //Serial.println(proximityVal);

  while (proximityVal == LOW) {
    delay(1500);
    if (digitalRead(proximitySensor) == LOW) {
      go();
    }
  }

}


void go() {
  proximityVal = digitalRead(proximitySensor);

  timer1.pause();
  timer2.pause();

  digitalWrite(relayUV, HIGH);
  digitalWrite(relayPTC, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Platform Opening...");
  do {
    Serial.println("Opening");
    digitalWrite(motorForward, HIGH);
    openVal = digitalRead(openSwitch);
  } while (openVal == HIGH);
  digitalWrite(motorForward, LOW);

  lcd.setCursor(0, 1);
  lcd.print("Platform Opened!");

  Serial.println("Opened");

  do {
    Serial.println("Standby");
    proximityVal = digitalRead(proximitySensor);
  } while (proximityVal == LOW);

  delay(1000);

  lcd.setCursor(0, 2);
  lcd.print("Platform Closing...");

  do {
    Serial.println("Closing");
    digitalWrite(motorBackward, HIGH);
    closeVal = digitalRead(closeSwitch);
  } while (closeVal == HIGH);
  digitalWrite(motorBackward, LOW);

  lcd.setCursor(0, 3);
  lcd.print("Platform Closed!");

  Serial.println("Closed");

  timer1.start();
  timer2.start();

  /*
     if (onPTC == true) {
     digitalWrite(relayPTC, LOW);
      }
      if (onUV == true) {
        digitalWrite(relayUV, LOW);
      }
  */

  digitalWrite(relayUV, LOW);
  digitalWrite(relayPTC, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EATING UTENSILS");
  lcd.setCursor(0, 1);
  lcd.print("STERILIZATION USING");
  lcd.setCursor(0, 2);
  lcd.print("UV WITH PTC HEATER.");
}


void uvFunction() {
  if (onUV) {
    digitalWrite(relayUV, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("UV Status: OFF");
  } else {
    digitalWrite(relayUV, LOW);

    lcd.setCursor(0, 1);
    lcd.print("UV Status: ON ");
  }

  onUV = !onUV;
}


void ptcFunction() {
  if (onPTC) {
    digitalWrite(relayPTC, HIGH);

    lcd.setCursor(0, 2);
    lcd.print("PTC Status: OFF");
  } else {
    digitalWrite(relayPTC, LOW);

    lcd.setCursor(0, 2);
    lcd.print("PTC Status: ON ");
  }

  onPTC = !onPTC;
}
