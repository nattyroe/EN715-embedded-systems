#include "MorseManager.hpp"
#include "MorseCode.hpp"

bool MorseManager::ledChangeNeeded(const unsigned long currentMillis)
{
    bool changeNeeded = false;
    if (currentMillis > this->nextUpdateTime)
    {
        if (!onBlink)
        {
            this->onBlink = true;
            this->nextUpdateTime = currentMillis + MorseCode::length::BLINK;
            changeNeeded = true;
        }
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
        else if (this->blinkIdx == MorseCode::MAX_MORSE_LENGTH - 1)
        {
            this->onBlink = true;
            ++this->currentLineIndex;
            ++this->blinkIdx;
            this->nextUpdateTime = currentMillis + MorseCode::length::GAP;
            changeNeeded = false;
        }
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
    if (message[currentLineIndex] == ' ')
    {
        changeNeeded = false;
    }
    return changeNeeded;
}

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

bool MorseManager::printComplete()
{
    bool letterComplete = this->blinkIdx >= MorseCode::MAX_MORSE_LENGTH;
    bool lineComplete = this->currentLineIndex >= this->currentLineLength;
    return letterComplete && lineComplete;
}
