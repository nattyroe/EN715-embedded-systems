
#include <SoftwareSerial.h>

const int thermistorPin = A0;
int temperatureVal;

const int logResetPin = 2;
const int txPin = 1;
//const int rxPin = 0; // Currently unused

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(logResetPin, OUTPUT);
    // Default state high, low to reset logging
    digitalWrite(logResetPin, HIGH);

}

void loop() {
    // put your main code here, to run repeatedly:
    temperatureVal = analogRead(thermistorPin);
    Serial.println(temperatureVal);

}

// Pull logger interrupt low
void resetLogger() {
    digitalWrite(logResetPin, LOW);
    delay(10);
    digitalWrite(logResetPin, HIGH);
}
