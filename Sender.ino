#include <SoftwareSerial.h>
SoftwareSerial hc12(2, 3); //(tx,rx)

const int lowSwitch = 5;
const int midSwitch = 6;
const int highSwitch = 7;

const int ledLow = 8;
const int ledMid = 9;
const int ledHigh = 10;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 300;

int lastLow = HIGH;
int lastMid = HIGH;
int lastHigh = HIGH;

int stableLow = HIGH;
int stableMid = HIGH;
int stableHigh = HIGH;

String lastState = "";

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);

  pinMode(lowSwitch, INPUT_PULLUP);
  pinMode(midSwitch, INPUT_PULLUP);
  pinMode(highSwitch, INPUT_PULLUP);

  pinMode(ledLow, OUTPUT);
  pinMode(ledMid, OUTPUT);
  pinMode(ledHigh, OUTPUT);
}

void loop() {
  int readingLow = digitalRead(lowSwitch);
  int readingMid = digitalRead(midSwitch);
  int readingHigh = digitalRead(highSwitch);

  if (readingLow != lastLow || readingMid != lastMid || readingHigh != lastHigh) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    stableLow = readingLow;
    stableMid = readingMid;
    stableHigh = readingHigh;
  }

  lastLow = readingLow;
  lastMid = readingMid;
  lastHigh = readingHigh;

  digitalWrite(ledLow, stableLow == LOW);
  digitalWrite(ledMid, stableMid == LOW);
  digitalWrite(ledHigh, stableHigh == LOW);

  if (stableLow == HIGH && stableMid == HIGH && stableHigh == HIGH) {
    if (lastState != "empty") {
      hc12.println("empty");
      lastState = "empty";
    }
  }

  if (stableLow == LOW && stableMid == LOW && stableHigh == LOW) {
    if (lastState != "full") {
      hc12.println("full");
      lastState = "full";
    }
  }

  if (Serial.available()) {
    hc12.write(Serial.read());
  }

  if (hc12.available()) {
    Serial.write(hc12.read());
  }
}