#include <SoftwareSerial.h>
SoftwareSerial hc12(2, 3);

const int relayPin = 8;

String buffer = "";
String lastState = "";

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {

  while (hc12.available()) {
    char c = hc12.read();

    if (c == '\n' || c == '\r') {
      
      buffer.trim();

      if (buffer == "empty") {
        if (lastState != "empty") {
          digitalWrite(relayPin, HIGH);
          lastState = "empty";
          Serial.println("PUMP ON (EMPTY)");
        }
      }

      if (buffer == "full") {
        if (lastState != "full") {
          digitalWrite(relayPin, LOW);
          lastState = "full";
          Serial.println("PUMP OFF (FULL)");
        }
      }

      buffer = "";
    } else {
      buffer += c;
    }
  }

}