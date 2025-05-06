#pragma once

#include "siebenuhr_core.h"
#include "siebenuhr_glyph.h"

#include <RunningAverage.h>
#include <FastLED.h>

#include "IDisplayRenderer.h"

#include "FX/snake.h"

#include <memory>
#include <string>
#include <vector>

namespace siebenuhr_core
{
    class Display 
    {
    public:
        static Display* getInstance();

        void initialize(ClockType clockType, int numGlyphs);
        void update();

        void setPowerEnabled(bool isEnabled);
        void setHeartbeatEnabled(bool isEnabled);
        void setDynEnabled(bool isEnabled);

        void setNotification(const std::string& text, int duration = 1500);

        void setEnvLightLevel(float lux, int baseBrightness = 10, int maxBrightnessRange = 255);

        void setBrightness(int value, bool saveToEEPROM = true);
        void setText(const std::string& text);
        void setColor(const CRGB& color, int steps = 0);
        
        void setTime(int hours, int minutes);
        void getTime(int& hours, int& minutes);

        CRGB getColor();
        int getBrightness();

        void setPersonality(PersonalityType personality);
        PersonalityType getCurrentPersonality() const { return m_currentPersonality; }
        void selectAdjacentPersonality(int direction);
        void setDefaultAnimationTime(unsigned long duration) { m_defaultAnimationTime = duration; }
        void startLEDAnimation(int ledIndex, const CRGB& targetColor, unsigned long duration = 0);

        // Get access to LED buffers for renderers
        CRGB* getLEDBuffer() const { return m_LEDs; }
        LEDAnimationState* getAnimationStates() const { return m_animationStates; }

    private:
        Display();
        ~Display() = default;

        void initializeGlyphs(int numSegments, int ledsPerSegment);
        bool setRenderer(std::unique_ptr<IDisplayRenderer> renderer);
        std::unique_ptr<IDisplayRenderer> createRenderer(PersonalityType personality, const CRGB& defaultColor);
        void updateLEDAnimations();

        static Display* s_instance;

        ClockType m_clockType;
        unsigned long m_lastUpdateTime = 0;

        // Heartbeat
        bool m_heartbeatEnabled = false;
        unsigned long m_lastHeartbeatTime = 0;
        unsigned long m_heartbeatInterval = 1000;
        bool m_heartbeatState = false;

        bool m_powerEnabled;
        int m_nBrightness;
        CRGB m_currentColor;

        // Glyphs        
        int m_numGlyphs;
        int m_numSegments;
        int m_numLEDsPerSegments;
        int m_numLEDs;
        Glyph** m_glyphs = nullptr;
       	CRGB *m_LEDs = nullptr;  // Current state (bound to FastLED)
        LEDAnimationState* m_animationStates = nullptr;  // Per-LED animation states
        unsigned long m_defaultAnimationTime = 0;  // Default animation time for new animations

        int m_hours = 0;
        int m_minutes = 0;  
        std::string m_text;
        int m_curTextPos = 0;

        // Notification
        int m_notificationDuration = 0; 
        unsigned long m_notificationStartTime = 0;
        bool m_notificationActive = false;
        CRGB m_notificationColor = constants::NotificationColor;
        int m_notificationBrightness = constants::NotificationBrightness;
        std::unique_ptr<IDisplayRenderer> m_notificationRenderer;  // Dedicated renderer for notifications

        // Previous state
        std::unique_ptr<IDisplayRenderer> m_previousRenderer;  // Store previous renderer to restore after notification
        int m_previousBrightness;  // Store previous brightness to restore after notification

        unsigned long m_lastUpdateMillis;
        RunningAverage m_avgComputionTime;
    
        PersonalityType m_currentPersonality;
        std::unique_ptr<IDisplayRenderer> m_renderer;
    };
}