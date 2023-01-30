
#include "MorseManager.hpp"

const int ledPin = LED_BUILTIN;
int ledState = LOW;

const unsigned int MAX_LINE_LENGTH = 20;
unsigned int linePosition = 0;
static char message[MAX_LINE_LENGTH];
const char *m = message;

const int CTRL_C = 3;
const int CTRL_Z = 26; 

MorseManager *morseManager = new MorseManager();

void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
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
            Serial.println(" Playing");
            break;
        }
        else if (readByte == CTRL_Z || readByte == CTRL_C)
        {
          exit(0);
        }
        else if (linePosition < MAX_LINE_LENGTH  && readByte >= 0)
        {
            message[linePosition++] = readByte;
            Serial.print(message[linePosition-1]);
        }
    }
}

void blinkLoop()
{
    while(true)
    {   
        if (morseManager->ledChangeNeeded(millis()))
        {
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
