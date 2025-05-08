#pragma once

#include "siebenuhr_core.h"

namespace siebenuhr_core {

    enum class ButtonState {
        Idle = 0,
        SingleClick,
        DoubleClick,
        LongClick,
        LongPress
    };

    class UIButton {
    public:
        UIButton(uint8_t buttonPin, uint8_t ledPin);
        ~UIButton() {}

        ButtonState update();
        ButtonState getState();
        bool isLongPress();
        bool isPressed();
        bool isReleased();

    private:
        uint8_t m_buttonPin;
        uint8_t m_ledPin;

        // Button state
        int m_state = HIGH;
        int m_lastState = HIGH;
        unsigned long m_lastTransition = 0;

        
        // Event flags and timer
        bool m_pressedEvent = false;
        bool m_releasedEvent = false;
        unsigned long m_lastPressEventTime = 0;
        unsigned long m_lastSingleClickTime = 0;

        
        ButtonState m_buttonState = ButtonState::Idle;

        // Timing constants
        static constexpr unsigned long DEBOUNCE_DELAY = 50;    // ms
        static constexpr unsigned long DOUBLE_CLICK_TIME = 350;  // ms
        static constexpr unsigned long LONG_PRESS_THRESHOLD = 500;  // ms
    };

}
