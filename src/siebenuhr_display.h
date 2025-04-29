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
    // Forward declarations
    class FixedColorRenderer;
    class ColorWheelRenderer;

    enum ClockType {
        CLOCK_TYPE_REGULAR = 0,
        CLOCK_TYPE_MINI
    };

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
        
        CRGB getColor();
        int getBrightness();

        void setPersonality(PersonalityType personality);

    private:
        Display();
        ~Display() = default;

        void initializeGlyphs(int numSegments, int ledsPerSegment);
        bool setRenderer(std::unique_ptr<IDisplayRenderer> renderer);
        std::unique_ptr<IDisplayRenderer> createRenderer(PersonalityType personality, const CRGB& defaultColor);

        static Display* s_instance;

        ClockType m_clockType;
        unsigned long m_lastUpdateTime = 0;

        // Heartbeat
        bool m_heartbeatEnabled = false;
        unsigned long m_lastHeartbeatTime = 0;
        unsigned long m_heartbeatInterval = 1000;
        bool m_heartbeatState = false;

        // Glyphs
        bool m_powerEnabled;
        
        int m_nBrightness;
        
        int m_numGlyphs;
        int m_numSegments;
        int m_numLEDsPerSegments;
        int m_numLEDs;
        Glyph** m_glyphs = nullptr;
       	CRGB *m_LEDs = nullptr;

        int m_hours = 0;
        int m_minutes = 0;  
        std::string m_text;
        int m_curTextPos = 0;

        std::string m_notificationText = "";
        int m_notificationDuration = 0; 

        unsigned long m_lastUpdateMillis;
        RunningAverage m_avgComputionTime;
    
        PersonalityType m_currentPersonality;
        std::unique_ptr<IDisplayRenderer> m_renderer;
};
}