#include "siebenuhr_encoder.h"

namespace siebenuhr_core
{

    #define ROTARY_ENCODER_STEPS 4

    AiEsp32RotaryEncoder* UIKnob::m_rotaryEncoder = nullptr;

    void IRAM_ATTR UIKnob::handleEncoderInterrupt() {
        m_rotaryEncoder->readEncoder_ISR();
    }

    void IRAM_ATTR UIKnob::handleButtonInterrupt() {
        m_rotaryEncoder->readButton_ISR();
    }

    UIKnob::UIKnob(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t buttonPin) 
    {
        m_rotaryEncoder = new AiEsp32RotaryEncoder(encoderPinA, encoderPinB, buttonPin, -1, ROTARY_ENCODER_STEPS);
        m_rotaryEncoder->begin();
        m_rotaryEncoder->setup(handleEncoderInterrupt, handleButtonInterrupt);
        m_rotaryEncoder->setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        m_rotaryEncoder->setAcceleration(250);

        m_encoderPosition = 0;
        m_encoderPositionDiff = 0;
        m_buttonPrevPressedState = false;
        m_buttonPressedTime = 0; 
    }

    UIKnob::~UIKnob() {
    delete m_rotaryEncoder;
    }

    void UIKnob::update() {
        m_encoderPositionDiff = m_rotaryEncoder->encoderChanged();
        m_encoderPosition = m_rotaryEncoder->readEncoder();	
        m_buttonPressedState = m_rotaryEncoder->isEncoderButtonDown();

        if (isButtonPressed()) {
            digitalWrite(constants::LED2_PIN, HIGH);
            if (!m_buttonPrevPressedState) {
                m_buttonPrevPressedState = true;
                m_buttonPressedTime = millis();
            }
        } else {
            digitalWrite(constants::LED2_PIN, LOW);
            m_buttonPrevPressedState = false;
        }	
    }

    void UIKnob::setEncoderBoundaries(long minEncoderValue, long maxEncoderValue, long position, bool circleValues) {
        m_rotaryEncoder->setBoundaries(minEncoderValue, maxEncoderValue, circleValues);
        setPosition(position);
    }

    bool UIKnob::hasPositionChanged() {
        return m_encoderPositionDiff != 0;
    }

    void UIKnob::setPosition(long position) {
        m_encoderPosition = position;
        m_encoderPositionDiff = 0;
        m_rotaryEncoder->setEncoderValue(m_encoderPosition);
    }

    long UIKnob::getPosition() {
        return m_encoderPosition;
    }

    long UIKnob::getPositionDiff() {
        return m_encoderPositionDiff;
    }

    bool UIKnob::isButtonPressed() {
        return m_buttonPressedState;
    }

    bool UIKnob::isButtonReleased() {
        return m_rotaryEncoder->isEncoderButtonClicked();
    }

    long UIKnob::getButtonPressTime() {
        if (isButtonPressed()) {
            return 	millis() - m_buttonPressedTime;
        }
        return 0l;
    }
}