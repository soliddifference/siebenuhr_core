#include "siebenuhr_encoder.h"

namespace siebenuhr_core
{

    #define ROTARY_ENCODER_STEPS 4

    AiEsp32RotaryEncoder* UIKnob::_pRotaryEncoder = nullptr;

    void IRAM_ATTR UIKnob::handleEncoderInterrupt() {
        _pRotaryEncoder->readEncoder_ISR();
    }

    void IRAM_ATTR UIKnob::handleButtonInterrupt() {
        _pRotaryEncoder->readButton_ISR();
    }

    UIKnob::UIKnob(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t buttonPin) 
    {
        _pRotaryEncoder = new AiEsp32RotaryEncoder(encoderPinA, encoderPinB, buttonPin, -1, ROTARY_ENCODER_STEPS);
        _pRotaryEncoder->begin();
        _pRotaryEncoder->setup(handleEncoderInterrupt, handleButtonInterrupt);
        _pRotaryEncoder->setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        _pRotaryEncoder->setAcceleration(250);

        _nEncoderPosition = 0;
        _nEncoderPositionDiff = 0;
        _bButtonPrevPressedState = false;
        _nButtonPressedTime = 0; 
    }

    UIKnob::~UIKnob() {
    delete _pRotaryEncoder;
    }

    void UIKnob::update() {
        _nEncoderPositionDiff = _pRotaryEncoder->encoderChanged();
        _nEncoderPosition = _pRotaryEncoder->readEncoder();	
        _bButtonPressedState = _pRotaryEncoder->isEncoderButtonDown();

        if (isButtonPressed()) {
            digitalWrite(constants::LED2_PIN, HIGH);
            if (!_bButtonPrevPressedState) {
                _bButtonPrevPressedState = true;
                _nButtonPressedTime = millis();
            }
        } else {
            digitalWrite(constants::LED2_PIN, LOW);
            _bButtonPrevPressedState = false;
        }	
    }

    void UIKnob::setEncoderBoundaries(long minEncoderValue, long maxEncoderValue, long position, bool circleValues) {
        _pRotaryEncoder->setBoundaries(minEncoderValue, maxEncoderValue, circleValues);
        setPosition(position);
    }

    bool UIKnob::hasPositionChanged() {
        return _nEncoderPositionDiff != 0;
    }

    void UIKnob::setPosition(long position) {
        _nEncoderPosition = position;
        _nEncoderPositionDiff = 0;
        _pRotaryEncoder->setEncoderValue(_nEncoderPosition);
    }

    long UIKnob::getPosition() {
        return _nEncoderPosition;
    }

    long UIKnob::getPositionDiff() {
        return _nEncoderPositionDiff;
    }

    bool UIKnob::isButtonPressed(int countThreshold) {
        return _bButtonPressedState;
    }

    bool UIKnob::isButtonReleased(int countThreshold) {
        return _pRotaryEncoder->isEncoderButtonClicked();
    }

    long UIKnob::getButtonPressTime() {
        if (isButtonPressed()) {
            return 	millis() - _nButtonPressedTime;
        }
        return 0l;
    }
}