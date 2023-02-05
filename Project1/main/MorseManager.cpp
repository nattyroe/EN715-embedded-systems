#include "MorseManager.hpp"
#include "MorseCode.hpp"

// Checks whether next LED state change time is met
// based on current Arduino time in milliseconds
bool MorseManager::ledChangeNeeded(const unsigned long currentMillis)
{
    // No LED change needed if next update time is not exceeded
    bool changeNeeded = false;
    // Next update time has been met, check whether conditions require LED update
    if (currentMillis > this->nextUpdateTime)
    {
        // Enter state between Dashes/Dots
        if (!onBlink)
        {
            this->onBlink = true;
            this->nextUpdateTime = currentMillis + MorseCode::length::BLINK;
            changeNeeded = true;
        }
        // Enter state for current character's next Dash or Dot
        else if (onBlink && this->blinkIdx < MorseCode::MAX_MORSE_LENGTH - 1)
        {
            this->onBlink = false;
            if (this->nextUpdateTime > 0)
            {
                ++this->blinkIdx;
            }
            this->nextUpdateTime = currentMillis + *morseCode->get(message[currentLineIndex], blinkIdx);
            if (currentMillis == this->nextUpdateTime)
            {
                this->onBlink = true;
                changeNeeded = false;
            }
            else
            {
                changeNeeded = true;
            }
        }
        // Enter state for Gap between characters
        else if (this->blinkIdx == MorseCode::MAX_MORSE_LENGTH - 1)
        {
            this->onBlink = true;
            ++this->currentLineIndex;
            ++this->blinkIdx;
            this->nextUpdateTime = currentMillis + MorseCode::length::GAP;
            changeNeeded = false;
        }
        // Move to next character and begin first Dash/Dot
        else if (this->currentLineIndex < currentLineLength)
        {
            this->onBlink = false;
            this->blinkIdx = 0;
            this->nextUpdateTime = currentMillis + *morseCode->get(message[currentLineIndex], blinkIdx);
            if (currentMillis == this->nextUpdateTime)
            {
                this->onBlink = true;
                changeNeeded = false;
            }
            else
            {
                changeNeeded = true;
            }
        }
    }
    // Leave LED alone if Space is active
    if (message[currentLineIndex] == ' ')
    {
        changeNeeded = false;
    }
    return changeNeeded;
}

// Copies char array to blink in morse, and reset all tracking vars
void MorseManager::addLine(const char *line, unsigned int length)
{
    this->nextUpdateTime = 0;
    this->onBlink = true;
    this->currentLineIndex = 0;
    this->blinkIdx = 0;

    const char *linePointer = line;
    unsigned int idx = 0;
    while (idx < length)
    {
        this->message[idx] = *line;
        ++line;
        ++idx;
    }
    this->currentLineLength = length;
}

// Checks wether current line blink sequence is complete
bool MorseManager::printComplete()
{
    bool letterComplete = this->blinkIdx >= MorseCode::MAX_MORSE_LENGTH;
    bool lineComplete = this->currentLineIndex >= this->currentLineLength;
    return letterComplete && lineComplete;
}
