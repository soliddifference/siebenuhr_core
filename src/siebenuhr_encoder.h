#pragma once

#include "siebenuhr_core.h"

#include <AiEsp32RotaryEncoder.h>

namespace siebenuhr_core {

    class UIKnob {
    public:
        UIKnob(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t buttonPin);
        ~UIKnob();

        void update();

        void setEncoderBoundaries(long minEncoderValue, long maxEncoderValue, long position, bool circleValues = false);

        bool hasPositionChanged();
        void setPosition(long position);
        long getPosition();
        long getPositionDiff();

        bool isButtonPressed(int countThreshold=1);
        bool isButtonReleased(int countThreshold=1);
        long getButtonPressTime();
        
        static void IRAM_ATTR handleEncoderInterrupt();
        static void IRAM_ATTR handleButtonInterrupt();

    private:
        bool _bButtonPressedState;
        bool _bButtonPrevPressedState;
        long _nButtonPressedTime; 
        long _nEncoderPosition;
        long _nEncoderPositionDiff;
        static AiEsp32RotaryEncoder *_pRotaryEncoder;
    };

}
