#include "siebenuhr_controller.h"

#include <Arduino.h>

static const int GLYPH_COUNT = 4;
static const int SEGMENT_COUNT = 7;
static const int MINI_LEDS_PER_SEGMENT = 4;
static const int REGU_LEDS_PER_SEGMENT = 11;

namespace siebenuhr_core
{
    Controller* Controller::s_instance = nullptr;

    Controller* Controller::getInstance()
    {
        if (Controller::s_instance == nullptr) 
        {
            Controller::s_instance = new Controller();
        }
        return Controller::s_instance;
    }

    void Controller::initialize(int clock_type) 
    {
        if (clock_type == 1) 
        {
            // setup mini clock
            m_display = new Display(GLYPH_COUNT, SEGMENT_COUNT, MINI_LEDS_PER_SEGMENT);
        } 
        else 
        {
            // setup regular clock
            m_display = new Display(GLYPH_COUNT, SEGMENT_COUNT, REGU_LEDS_PER_SEGMENT);
        }

        // heartbeat
        m_previousMillis = 0;
        m_interval = 500;
        m_ledState = false;
        pinMode(m_ledPin, OUTPUT);
    }

    void Controller::setInterval(int interval) 
    {
        m_interval = interval;
    }

    void Controller::setLEDPin(int pin) 
    {
        m_ledPin = pin;
    }

    void Controller::update() 
    {
        unsigned long currentMillis = millis();
        if (currentMillis - m_previousMillis >= m_interval) {
            m_previousMillis = currentMillis;
            m_ledState = !m_ledState;
            digitalWrite(m_ledPin, m_ledState);
        }
    }
}