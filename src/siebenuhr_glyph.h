#pragma once

#include "siebenuhr_core.h"
#include "siebenuhr_effect.h"

namespace siebenuhr_core
{
    const int DIGIT[10][7] = {
        {1, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1},
        {1, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 0},
        {1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 1, 1, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0}    
    };

    // Implementation of glyph '67
    const int ASCII_TABLE[127][7] = {
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  0
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  1
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  2
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  3
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  4
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  5
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  6
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  7
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  8
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  9
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  10
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  11
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  12
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  13
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  14
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  15
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  16
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  17
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  18
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  19
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  20
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  21
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  22
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  23
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  24
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  25
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  26
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  27
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  28
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  29
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  30
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  31
        {0, 0, 0, 0, 0, 0, 0},  // ASCII  32 - SPACE
        {0, 0, 1, 0, 0, 0, 1},  // ASCII  33 - !
        {0, 0, 1, 1, 0, 0, 0},  // ASCII  34 - "
        {0, 1, 1, 1, 1, 0, 0},  // ASCII  35 - #
        {0, 1, 1, 0, 1, 1, 1},  // ASCII  36 - $
        {1, 1, 1, 1, 1, 1, 1},  // ASCII  37 - %
        {1, 1, 1, 0, 1, 1, 1},  // ASCII  38 - &
        {0, 0, 1, 0, 0, 0, 0},  // ASCII  39 - '
        {0, 0, 1, 0, 0, 0, 1},  // ASCII  40 - (
        {1, 0, 0, 1, 0, 0, 0},  // ASCII  41 - )
        {1, 1, 1, 1, 0, 0, 1},  // ASCII  42 - *
        {0, 1, 1, 0, 0, 0, 1},  // ASCII  43 - +
        {0, 0, 0, 0, 0, 0, 1},  // ASCII  44 - ,
        {0, 1, 0, 0, 0, 0, 0},  // ASCII  45 - -
        {0, 0, 0, 0, 0, 0, 1},  // ASCII  46 - .
        {0, 1, 0, 1, 0, 0, 1},  // ASCII  47 - /
        {1, 0, 1, 1, 1, 1, 1},  // ASCII  48 - 0
        {1, 0, 0, 1, 0, 0, 0},  // ASCII  49 - 1
        {0, 1, 0, 1, 1, 1, 1},  // ASCII  50 - 2
        {1, 1, 0, 1, 1, 1, 0},  // ASCII  51 - 3
        {1, 1, 1, 1, 0, 0, 0},  // ASCII  52 - 4
        {1, 1, 1, 0, 1, 1, 0},  // ASCII  53 - 5
        {1, 1, 1, 0, 1, 1, 1},  // ASCII  54 - 6
        {1, 0, 0, 1, 1, 0, 0},  // ASCII  55 - 7
        {1, 1, 1, 1, 1, 1, 1},  // ASCII  56 - 8
        {1, 1, 1, 1, 1, 1, 0},  // ASCII  57 - 9
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  58 - :
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  59 - ;
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  60 - <
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  61 - =
        {0, 1, 0, 0, 0, 1, 0},  // ASCII  62 - >
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  63 - ?
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  64 - @
        {1, 1, 1, 1, 1, 0, 1},  // ASCII  65 - A
        {1, 1, 1, 1, 1, 1, 1},  // ASCII  66 - B
        {0, 0, 1, 0, 1, 1, 1},  // ASCII  67 - C
        {1, 0, 1, 1, 1, 1, 1},  // ASCII  68 - D
        {0, 1, 1, 0, 1, 1, 1},  // ASCII  69 - E
        {0, 1, 1, 0, 1, 0, 1},  // ASCII  70 - F
        {1, 0, 1, 0, 1, 1, 1},  // ASCII  71 - G
        {1, 1, 1, 1, 0, 0, 1},  // ASCII  72 - H
        {1, 0, 0, 1, 0, 0, 0},  // ASCII  73 - I
        {1, 0, 0, 3, 0, 1, 1},  // ASCII  74 - J
        {1, 1, 1, 0, 1, 0, 1},  // ASCII  75 - K
        {0, 0, 1, 0, 0, 1, 1},  // ASCII  76 - L
        {1, 0, 0, 0, 1, 0, 1},  // ASCII  77 - M
        {1, 0, 1, 1, 1, 0, 1},  // ASCII  78 - N
        {1, 0, 1, 1, 1, 1, 1},  // ASCII  79 - O
        {0, 1, 1, 1, 1, 0, 1},  // ASCII  80 - P
        {0, 1, 1, 1, 1, 1, 0},  // ASCII  81 - Q
        {0, 0, 1, 1, 1, 0, 1},  // ASCII  82 - R
        {1, 1, 1, 0, 1, 1, 0},  // ASCII  83 - S
        {0, 1, 1, 0, 0, 1, 1},  // ASCII  84 - T
        {1, 0, 1, 1, 0, 1, 1},  // ASCII  85 - U
        {1, 0, 1, 1, 0, 1, 1},  // ASCII  86 - V
        {0, 0, 1, 1, 0, 1, 0},  // ASCII  87 - W
        {1, 1, 1, 1, 0, 0, 1},  // ASCII  88 - X
        {1, 1, 1, 1, 0, 1, 0},  // ASCII  89 - Y
        {0, 1, 0, 1, 1, 1, 1},  // ASCII  90 - Z
        {0, 0, 1, 0, 1, 1, 1},  // ASCII  91 - [
        {1, 1, 1, 0, 0, 0, 0},  // ASCII  92 - <backslash>
        {1, 0, 0, 1, 1, 1, 0},  // ASCII  93 - ]
        {0, 0, 1, 1, 1, 0, 0},  // ASCII  94 - ^
        {0, 0, 0, 0, 0, 1, 0},  // ASCII  95 - _
        {0, 0, 0, 1, 0, 0, 0},  // ASCII  96 - '
        {1, 1, 0, 1, 1, 1, 1},  // ASCII  97 - a
        {1, 1, 1, 0, 0, 1, 1},  // ASCII  98 - b
        {0, 1, 0, 0, 0, 1, 1},  // ASCII  99 - c
        {1, 1, 0, 1, 0, 1, 1},  // ASCII 100 - d
        {0, 1, 1, 1, 1, 1, 1},  // ASCII 101 - e
        {0, 1, 1, 0, 1, 0, 1},  // ASCII 102 - f
        {1, 1, 1, 1, 1, 1, 0},  // ASCII 103 - g
        {1, 1, 1, 0, 0, 0, 1},  // ASCII 104 - h
        {0, 0, 0, 0, 0, 0, 1},  // ASCII 105 - i
        {1, 0, 0, 0, 0, 1, 0},  // ASCII 106 - j
        {1, 1, 1, 0, 1, 0, 1},  // ASCII 107 - k
        {0, 0, 1, 0, 0, 0, 1},  // ASCII 108 - l
        {1, 0, 0, 0, 0, 0, 1},  // ASCII 109 - m
        {1, 1, 0, 0, 0, 0, 1},  // ASCII 110 - n
        {1, 1, 0, 0, 0, 1, 1},  // ASCII 111 - o
        {0, 1, 1, 1, 1, 0, 1},  // ASCII 112 - p
        {1, 1, 1, 1, 1, 0, 0},  // ASCII 113 - q
        {0, 1, 0, 0, 0, 0, 1},  // ASCII 114 - r
        {1, 1, 1, 0, 1, 1, 0},  // ASCII 115 - s
        {0, 1, 1, 0, 0, 1, 1},  // ASCII 116 - t
        {1, 0, 0, 0, 0, 1, 1},  // ASCII 117 - u
        {1, 0, 0, 0, 0, 1, 1},  // ASCII 118 - v
        {1, 0, 0, 0, 0, 0, 1},  // ASCII 119 - w
        {1, 1, 1, 1, 0, 0, 1},  // ASCII 120 - x
        {1, 1, 1, 1, 0, 1, 0},  // ASCII 121 - y
        {0, 1, 0, 1, 1, 1, 1},  // ASCII 122 - z
        {0, 0, 1, 0, 1, 1, 1},  // ASCII 123 - {
        {0, 0, 1, 0, 0, 0, 1},  // ASCII 124 - |
        {1, 0, 0, 1, 1, 1, 0},  // ASCII 125 - }
        {0, 1, 0, 0, 0, 0, 0}
    }; // ASCII 126 - ~

    class Glyph
    {
    public:
        Glyph(int numSegments, int numLEDsPerSegments);
        ~Glyph() = default;

        void attach(int glyphID, int glyphCount);
        void update(unsigned long currentMillis, CRGB *LEDs);

        void setEffect(Effect *effect);

    private:
        int m_numLEDS;
        CRGB m_colorBase;

        int m_glyphID;
        int m_glyphOffset;

        int m_numSegments;
        int m_numLEDsPerSegments;

        // test
        int char_id = 0;

        Effect *m_effect;
    };
}