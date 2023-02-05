#pragma once

#include "MorseCode.hpp"

using namespace std;

// MorseManager for tracking Morse Code sequence durations
//
// Starts blink time tracking when line is added using
// addLine; status updates can be tracked using ledChangeNeeded
// and using printComplete to verify all characters blinked.
class MorseManager
{

public:
    MorseManager(){};

    // Copies char array to blink in morse, and reset all tracking vars
    void addLine(const char *line, unsigned int length);

    // Checks whether next LED update change time is exceeded
    // based on current Arduino time in milliseconds
    bool ledChangeNeeded(const unsigned long currentMillis);

    // Checks wether current line blink sequence is complete
    bool printComplete();

    // Getters for internal variables, used for debugging Morse sequence timing
    unsigned long getNextUpdateTime() { return this->nextUpdateTime; };
    unsigned int getLineLength() { return sizeof(this->message) / sizeof(char); };
    unsigned int getCurrentLineLength() { return this->currentLineLength; };
    unsigned int getCurrentLineIndex() { return this->currentLineIndex; };
    bool getOnBlink() { return this->onBlink; };
    unsigned int getBlinkIdx() { return this->blinkIdx; };

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
