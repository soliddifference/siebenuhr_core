#pragma once

#include "siebenuhr_core.h"

namespace siebenuhr_core {

    enum class ClickType {
        None,
        Single,
        Double
    };

    class UIButton {
    public:
        UIButton(uint8_t buttonPin, uint8_t ledPin);
        ~UIButton() {}

        void update();

        ClickType getClickType();         // Single/Double
        bool isLongPress();               // True as long as button is held beyond long press threshold
        bool isPressed();                 // True only during press event
        bool isReleased();               // True only during release event

    private:
        uint8_t m_buttonPin;
        uint8_t m_ledPin;

        int m_state = HIGH;
        int m_lastState = HIGH;
        unsigned long m_lastDebounceTime = 0;

        unsigned long m_pressStartTime = 0;
        unsigned long m_lastReleaseTime = 0;
        unsigned long m_lastPressTime = 0;

        bool m_pressedEvent = false;
        bool m_releasedEvent = false;
        bool m_longPressActive = false;
        bool m_waitingForSecondClick = false;

        ClickType m_clickType = ClickType::None;

        static constexpr unsigned long DEBOUNCE_DELAY = constants::BUTTON_DEBOUNCE_DELAY;
        static constexpr unsigned long DOUBLE_CLICK_TIME = 400;
        static constexpr unsigned long LONG_PRESS_THRESHOLD = 700;
    };

}
