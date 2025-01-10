#pragma once

#include "siebenuhr_effect.h"

#include <vector>

namespace siebenuhr_core
{
    // Segment adjacency map with entry/exit alignment
    struct SegmentConnection {
        size_t nextSegment;
        bool entryDirection;
    };

    class SnakeFX : public Effect
    {
        public:
            SnakeFX(int numLEDs, int numLEDsPerSegments);

            void initialize();
            void update(CRGB *m_LEDs);

        protected:
            int m_numLEDs;
            int m_numLEDsPerSegments;

            size_t m_curSegment;
            size_t m_curLEDPos;
            bool m_directionUp;

            int *m_LEDCols;

            size_t getLEDIndex(size_t segment, size_t ledPos);
            void selectNextSegment(std::vector<std::vector<SegmentConnection>> &map);
    };
}