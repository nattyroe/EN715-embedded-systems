#pragma once

class MorseCode
{
public:
    enum length
    {
        NA = 0,
        BLINK = 100,
        SHORT = 200,
        GAP = 300,
        LONG = 600,
        // SPACE = 750
    };

    // static const int ARRAY_LENGTH = 38;
    static const int ARRAY_LENGTH = 37;
    static const int MAX_MORSE_LENGTH = 5;
    static const int SPACE_IDX = 36;

    MorseCode::length alphabet[ARRAY_LENGTH][MAX_MORSE_LENGTH] = {
        {LONG, LONG, LONG, LONG, LONG},      // 0
        {SHORT, LONG, LONG, LONG, LONG},     // 1
        {SHORT, SHORT, LONG, LONG, LONG},    // 2
        {SHORT, SHORT, SHORT, LONG, LONG},   // 3
        {SHORT, SHORT, SHORT, SHORT, LONG},  // 4
        {SHORT, SHORT, SHORT, SHORT, SHORT}, // 5
        {LONG, SHORT, SHORT, SHORT, SHORT},  // 6
        {LONG, LONG, SHORT, SHORT, SHORT},   // 7
        {LONG, LONG, LONG, SHORT, SHORT},    // 8
        {LONG, LONG, LONG, LONG, SHORT},     // 9
        {SHORT, LONG, NA, NA, NA},           // A
        {LONG, SHORT, SHORT, SHORT, NA},     // B
        {LONG, SHORT, LONG, SHORT, NA},      // C
        {LONG, SHORT, SHORT, NA, NA},        // D
        {SHORT, NA, NA, NA, NA},             // E
        {SHORT, SHORT, LONG, SHORT, NA},     // F
        {LONG, LONG, SHORT, NA, NA},         // G
        {SHORT, SHORT, SHORT, SHORT, NA},    // H
        {SHORT, SHORT, NA, NA, NA},          // I
        {SHORT, LONG, LONG, LONG, NA},       // J
        {LONG, SHORT, LONG, NA, NA},         // K
        {SHORT, LONG, SHORT, SHORT, NA},     // L
        {LONG, LONG, NA, NA, NA},            // M
        {LONG, SHORT, NA, NA, NA},           // N
        {LONG, LONG, LONG, NA, NA},          // O
        {SHORT, LONG, LONG, SHORT, NA},      // P
        {LONG, LONG, SHORT, LONG, NA},       // Q
        {SHORT, LONG, SHORT, NA, NA},        // R
        {SHORT, SHORT, SHORT, NA, NA},       // S
        {LONG, NA, NA, NA, NA},              // T
        {SHORT, SHORT, LONG, NA, NA},        // U
        {SHORT, SHORT, SHORT, LONG, NA},     // V
        {SHORT, LONG, LONG, NA, NA},         // W
        {LONG, SHORT, SHORT, LONG, NA},      // X
        {LONG, SHORT, LONG, LONG, NA},       // Y
        {LONG, LONG, SHORT, SHORT, NA},      // Z
        // {SPACE, NA, NA, NA, NA},             // SPACE
        {NA, NA, NA, NA, NA} // BAD VALUE
    };

    MorseCode::length *get(char letter, unsigned int idx)
    {
        int charVal = int(letter);
        if (letter >= '0' && charVal <= '9')
        {
            charVal = charVal - int('0');
        }
        else if (letter >= 'A' && letter <= 'Z')
        {
            charVal = charVal - int('A') + 10;
        }
        else if (letter >= 'a' && letter <= 'z')
        {
            charVal = charVal - int('a') + 10;
        }
        // else if (letter == ' ')
        // {
        //     charVal = SPACE_IDX;
        // }
        else
        {
            charVal = ARRAY_LENGTH;
        }

        return &alphabet[charVal][idx];
    };
};