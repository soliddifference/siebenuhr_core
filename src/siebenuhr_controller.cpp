#include "siebenuhr_controller.h"
#include <Arduino.h>

namespace siebenuhr_core {

enum class ClockType {
    Mini,
    Regular
};

Controller* Controller::s_instance = nullptr;

Controller* Controller::getInstance() {
    if (Controller::s_instance == nullptr) {
        Controller::s_instance = new Controller();
    }
    return Controller::s_instance;
}

void Controller::initialize(ClockType clockType) {
    if (clockType == ClockType::Mini) {
        // Setup mini clock
        m_display = new Display(constants::GlyphCount, constants::SegmentCount, constants::MiniLedsPerSegment);
    } else {
        // Setup regular clock
        m_display = new Display(constants::GlyphCount, constants::SegmentCount, constants::RegularLedsPerSegment);
    }

    // Heartbeat setup
    pinMode(m_heartbeatPin, OUTPUT);
}

void Controller::setHeartbeatEnabled(bool isEnabled) {
    m_heartbeatEnabled = isEnabled;
}

void Controller::update() {
    unsigned long currentMillis = millis();
    if (m_heartbeatEnabled && (currentMillis - m_lastHeartbeatTime >= m_heartbeatInterval)) {
        m_lastHeartbeatTime = currentMillis;
        m_heartbeatState = !m_heartbeatState;
        digitalWrite(m_heartbeatPin, m_heartbeatState);
    }
}

} // namespace siebenuhr_core
