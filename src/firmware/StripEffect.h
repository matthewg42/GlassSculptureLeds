#pragma once

#include <FastLED.h>

class StripEffect {
public:
    typedef enum {
        In,
        On,
        Out,
        Off
    } FadeState;

public:
    /*! Constructor */
    StripEffect(CRGB* ledData, uint16_t numLeds, uint16_t updatePeriodMs=30);
    /*! Destructor */
    virtual ~StripEffect();
    /*! Trigger fade in of effect */
    virtual void fadeIn(uint16_t durationMs);
    /*! Trigger fade out of effect */
    virtual void fadeOut(uint16_t durationMs);
    /*! Call this regularly from your code */
    virtual void update();
    /*! Implement this to draw the effect on the strip */
    virtual void affect() = 0;
    /*! \return a value between 0 and 1 showing how faded the display is. */
    float fadeRatio();
    /*! \return what state we are in with respect to fading */
    FadeState fadeState();

protected:
    CRGB* _ledData;
    uint16_t _numLeds;
    uint32_t _lastUpdateMs;
    uint16_t _updatePeriodMs;
    FadeState _fadeState;   
    uint32_t _fadeStart;
    uint32_t _fadeDurationMs;

};

