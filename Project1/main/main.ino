
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
    Serial.println("Input Loop");
    while(true)
    {
        char readByte = Serial.read();
        if (readByte == '\n')
        {
            morseManager->addLine(m, linePosition);
            linePosition = 0;
            Serial.println("Sent Line");
            break;
        }
        else if (linePosition < MAX_LINE_LENGTH  && readByte != startChar)
        {
            message[linePosition++] = readByte;
            Serial.print("Char added: ");
            Serial.println(readByte);
        }
    }
}

void blinkLoop()
{
    Serial.println("Blink Loop");
    while(true)
    {
        Serial.println();
        Serial.print("Current Time: ");
        unsigned long currentTime = millis();
        Serial.println(currentTime);
        Serial.println(morseManager->getBlinkIdx());
        
        if (morseManager->ledChangeNeeded(currentTime))
        {
            // if the LED is off turn it on and vice-versa
            changeLedState();
        }
        Serial.print("Number of Chars: ");
        Serial.println(morseManager->getCurrentLineLength());
        Serial.print("Next Update: ");
        Serial.println(morseManager->getNextUpdateTime());
        Serial.print("Letter Index: ");
        Serial.println(morseManager->getCurrentLineIndex());
        Serial.print("Blink Index: ");
        Serial.println(morseManager->getBlinkIdx());
        Serial.print("Current Letter: ");
        Serial.println(morseManager->getCurrentChar());
        Serial.print("Current Duration: ");
        Serial.println(morseManager->getCurrentDuration());
        Serial.print("On Blink?: ");
        Serial.println(morseManager->getOnBlink());
        
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
            Serial.println("Turning On");
        }
        else
        {
            ledState = LOW;
            Serial.println("Turning Off");
        }
        digitalWrite(ledPin, ledState);
}
