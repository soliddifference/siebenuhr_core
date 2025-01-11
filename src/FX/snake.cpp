#include <Arduino.h>

#include "snake.h"

namespace siebenuhr_core
{
    /*
    --- <4 ---
    |        |
    2u       3d
    |        |
    --- 1>---
    |        |
    6u       0u
    |        |
    --- 5> ---
    */

    std::vector<std::vector<SegmentConnection>> adjacencyMap = {
        {{1, false}, {3, false}},   // Segment 0 neighbors
        {{0, false}, {3, false}},   // Segment 1 neighbors
        {{4, false}},               // Segment 2 neighbors
        {{1, false}, {0, false}},   // Segment 3 neighbors
        {{2, false}},               // Segment 4 neighbors
        {{0, true}},                // Segment 5 neighbors
        {{1, true}, {2, true}}      // Segment 6 neighbors
    };

    std::vector<std::vector<SegmentConnection>> adjacencyMapRev = {
        {{5, false}},               // Segment 0 neighbors
        {{2, true}, {6, false}},    // Segment 1 neighbors
        {{1, true}, {6, false}},    // Segment 2 neighbors
        {{4, true}},                // Segment 3 neighbors
        {{3, true}},                // Segment 4 neighbors
        {{6, true}},                // Segment 5 neighbors
        {{5, true}}                 // Segment 6 neighbors
    }; 

    SnakeFX::SnakeFX(int numLEDs, int numLEDsPerSegments) 
        : m_numLEDs(numLEDs)
        , m_numLEDsPerSegments(numLEDsPerSegments)
    {       
        m_LEDCols = new int[m_numLEDs];
        memset(m_LEDCols, 0, sizeof(int)*m_numLEDs); 
    }

    void SnakeFX::initialize() 
    {
        m_curLEDPos = 0;
        m_curSegment = 0;
        m_directionUp = true;
    }

    void SnakeFX::update(unsigned long currentMillis, CRGB *LEDs)
    {
        // Dim all LEDs
        for (size_t i = 0; i < m_numLEDs; ++i) 
        {
            m_LEDCols[i] = m_LEDCols[i] >> 1; // Smooth fade
        }

        // Light up the current LED
        size_t ledIndex = getLEDIndex(m_curSegment, m_curLEDPos);
        m_LEDCols[ledIndex] = 255;

        // Move within the current segment
        if (m_directionUp) {
            m_curLEDPos++;
            if (m_curLEDPos >= m_numLEDsPerSegments) 
            { 
                // Exit the segment at the top
                selectNextSegment(adjacencyMap);
            }
        } else {
            if (m_curLEDPos > 0) {
                m_curLEDPos--;
            } else {
                // Exit the segment at the bottom
                selectNextSegment(adjacencyMapRev);
            }
        }

        // Update the physical LEDs
        for (size_t i = 0; i < m_numLEDs; ++i) 
        {
            // LEDs[i] = CRGB(m_LEDCols[i], m_LEDCols[i], m_LEDCols[i]);

            LEDs[m_glyphOffset + i].r = m_LEDCols[i];
            LEDs[m_glyphOffset + i].g = m_LEDCols[i];
            LEDs[m_glyphOffset + i].b = m_LEDCols[i];
        }
    }

    size_t SnakeFX::getLEDIndex(size_t segment, size_t ledPos) {
        if (segment % 2 == 0) { // Even segments: bottom-up order
            return segment * m_numLEDsPerSegments + ledPos;
        } else { // Odd segments: top-down order
            return segment * m_numLEDsPerSegments + (m_numLEDsPerSegments - 1 - ledPos);
        }
    }

    void SnakeFX::selectNextSegment(std::vector<std::vector<SegmentConnection>> &map) 
    {
        // Choose the next segment based on adjacency
        const auto &connections = map[m_curSegment];
        const auto &nextConnection = connections[std::rand() % connections.size()];
        m_curSegment = nextConnection.nextSegment;
        m_directionUp = nextConnection.entryDirection;

        if (m_directionUp) 
        {
            m_curLEDPos = 0;
        } 
        else
        {
            m_curLEDPos = m_numLEDsPerSegments - 1;
        }
    }
}