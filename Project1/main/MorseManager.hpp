#pragma once

#include "MorseCode.hpp"

using namespace std;

class MorseManager
{

public:
    MorseManager(){};
    bool ledChangeNeeded(const unsigned long currentMillis);

    void addLine(const char *line, unsigned int length);

    bool printComplete();
    unsigned int getLineLength() { return sizeof(this->message) / sizeof(char); };
    unsigned int getCurrentLineLength() { return this->currentLineLength; };
    unsigned long getNextUpdateTime() { return this->nextUpdateTime; };
    unsigned int getCurrentLineIndex() { return this->currentLineIndex; };
    unsigned int getBlinkIdx() { return this->blinkIdx; };
    char getCurrentChar() { return this->message[this->currentLineIndex]; };
    unsigned int getCurrentDuration() { return *morseCode->get(message[currentLineIndex], blinkIdx); };
    bool getOnBlink() { return this->onBlink; };

private:
    MorseCode *morseCode = new MorseCode();
    unsigned long nextUpdateTime = 0;

    static const unsigned int MAX_LINE_LENGTH = 20;
    unsigned int currentLineLength = 0;
    unsigned int currentLineIndex = 0;
    char message[MAX_LINE_LENGTH];

    bool onBlink = true;

    unsigned int blinkIdx = 0;
};