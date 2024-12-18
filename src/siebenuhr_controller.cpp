#include "siebenuhr_controller.h"
#include <Arduino.h>

const int LED_PIN = 2;

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

    void Controller::initialize() 
    {
        m_previousMillis = 0;
        m_interval = 500;
        m_ledState = false;

        pinMode(LED_PIN, OUTPUT);
    }

    void Controller::update() 
    {
        unsigned long currentMillis = millis();
        if (currentMillis - m_previousMillis >= m_interval) {
            m_previousMillis = currentMillis;
            m_ledState = !m_ledState;
            digitalWrite(LED_PIN, m_ledState);
        }
    }
}