
#include <SoftwareSerial.h>

const int thermistorPin = A0;
unsigned long avgTempVal = 0;
unsigned int tempCount = 0;

unsigned long logInterval = 10000;
unsigned long nextLogTime = millis() + logInterval;

//const int rxPin = 0; // Currently unused
const int txPin = 1;
const int logResetPin = 2;
//const int interruptPin = 3;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(logResetPin, OUTPUT);
    // Default state high, low to reset logging
    digitalWrite(logResetPin, HIGH);

//    attachInterrupt(interruptPin, logAvgTemp, HIGH);

    Serial.print("Time (s), Temp (F)\n");

}

void loop() {
    unsigned long prevAvg = avgTempVal * tempCount;
    ++tempCount;
    avgTempVal = (prevAvg + analogRead(thermistorPin)) / tempCount;

    if (millis() > nextLogTime){
        //digitalWrite(interruptPin, HIGH);
        logAvgTemp();
    }
}

float tempValToFarenheit(unsigned int tempVal)
{
  return (float) tempVal;
}

void logAvgTemp()
{
    Serial.print(millis()/1000);
    Serial.print(", ");
    Serial.print(tempValToFarenheit(avgTempVal));
    Serial.print(", ");
    Serial.print(avgTempVal);
    Serial.print("\n");
    avgTempVal = 0;
    tempCount = 0;
    nextLogTime = millis() + logInterval;
//    digitalWrite(interruptPin, LOW);
}

// Pull logger interrupt low
void resetLogger() {
    digitalWrite(logResetPin, LOW);
    delay(10);
    digitalWrite(logResetPin, HIGH);
}
