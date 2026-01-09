#pragma once

#include "siebenuhr_core.h"

#include <AiEsp32RotaryEncoder.h>

namespace siebenuhr_core {

    class UIKnob {
    public:
        // feedbackLedPin: GPIO pin for button press feedback LED, or -1 to disable
        UIKnob(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t buttonPin, int8_t feedbackLedPin = -1);
        ~UIKnob();

        void update();

        void setEncoderBoundaries(long minEncoderValue, long maxEncoderValue, long position, bool circleValues = false);

        bool hasPositionChanged();
        void setPosition(long position);
        long getPosition();
        long getPositionDiff();

        bool isButtonPressed();
        bool isButtonReleased();
        long getButtonPressTime();
        
        static void IRAM_ATTR handleEncoderInterrupt();
        static void IRAM_ATTR handleButtonInterrupt();

    private:
        int8_t m_feedbackLedPin;
        bool m_buttonPressedState;
        bool m_buttonPrevPressedState;
        long m_buttonPressedTime; 
        long m_encoderPosition;
        long m_encoderPositionDiff;
        static AiEsp32RotaryEncoder *m_rotaryEncoder;
    };

}
