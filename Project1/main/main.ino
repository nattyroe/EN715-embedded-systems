
#include "MorseManager.hpp"

const int ledPin = LED_BUILTIN; // the number of the LED pin
unsigned long previousMillis = 0;

int ledState = LOW;
const unsigned long interval = 1000;

const unsigned int MAX_LINE_LENGTH = 20;
unsigned int linePosition = 0;
static char message[MAX_LINE_LENGTH];
const char *m = message;
char startChar;

MorseManager *morseManager = new MorseManager();

void setup()
{
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    startChar = Serial.read();
}

void loop()
{
    inputLoop();
    blinkLoop();
}

void inputLoop()
{
    Serial.println("Please Provide Input");
    while(true)
    {
        char readByte = Serial.read();
        if (readByte == '\n')
        {
            morseManager->addLine(m, linePosition);
            linePosition = 0;
            break;
        }
        else if (linePosition < MAX_LINE_LENGTH  && readByte != startChar)
        {
            message[linePosition++] = readByte;
        }
    }
}

void blinkLoop()
{
    while(true)
    {   
        if (morseManager->ledChangeNeeded(millis()))
        {
            // if the LED is off turn it on and vice-versa
            changeLedState();
        }
        
        if (morseManager->printComplete())
        {
          break;
        }
    }
}

void changeLedState()
{
        if (ledState == LOW)
        {
            ledState = HIGH;
        }
        else
        {
            ledState = LOW;
        }
        digitalWrite(ledPin, ledState);
}
