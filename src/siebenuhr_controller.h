#pragma once

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"

namespace siebenuhr_core {

class Controller {
public:
    enum class ClockType {
        Mini,
        Regular
    };

    static Controller* getInstance();

    void initialize(ClockType clockType, int numGlyphs);
    void update();

    void setHeartbeatEnabled(bool isEnabled);

private:
    Controller() = default;

    static Controller* s_instance;

    Display* m_display = nullptr;

    // Heartbeat control
    bool m_heartbeatEnabled = false;
    int m_heartbeatPin = constants::PinHeartbeat;
    unsigned long m_lastHeartbeatTime = 0;
    unsigned long m_heartbeatInterval = 1000;
    bool m_heartbeatState = false;
};

} // namespace siebenuhr_core
