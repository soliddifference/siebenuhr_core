#include "siebenuhr_controller.h"
#include <Arduino.h>

namespace siebenuhr_core {

enum class ClockType {
    Mini,
    Regular
};

Controller* Controller::s_instance = nullptr;

Controller* Controller::getInstance() 
{
    if (Controller::s_instance == nullptr) {
        Controller::s_instance = new Controller();
    }
    return Controller::s_instance;
}

void Controller::initialize(ClockType clockType, int numGlyphs) 
{
    int count = numGlyphs == -1 ? constants::GlyphCount : numGlyphs;

    if (clockType == ClockType::Mini) {
        // Setup mini clock
        m_display = new Display(numGlyphs, constants::SegmentCount, constants::MiniLedsPerSegment);
    } else {
        // Setup regular clock
        m_display = new Display(numGlyphs, constants::SegmentCount, constants::RegularLedsPerSegment);
    }

    // Heartbeat setup
    pinMode(m_heartbeatPin, OUTPUT);
}

void Controller::setHeartbeatEnabled(bool isEnabled) 
{
    m_heartbeatEnabled = isEnabled;
}

void Controller::update() 
{
    m_display->update();

    unsigned long currentMillis = millis();
    if (m_heartbeatEnabled && (currentMillis - m_lastHeartbeatTime >= m_heartbeatInterval)) {
        m_lastHeartbeatTime = currentMillis;
        m_heartbeatState = !m_heartbeatState;
        digitalWrite(m_heartbeatPin, m_heartbeatState);
    }
}

} // namespace siebenuhr_core
