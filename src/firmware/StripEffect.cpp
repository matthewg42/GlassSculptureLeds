#include <Millis.h>
#include "StripEffect.h"

StripEffect::StripEffect(CRGB* ledData, uint16_t updatePeriodMs) :
    _ledData(ledData),
    _lastUpdateMs(0),
    _updatePeriodMs(updatePeriodMs),
    _fadeState(Off),
    _fadeStart(0),
    _fadeDurationMs(0)
{
}

StripEffect::~StripEffect() 
{
}

void StripEffect::fadeIn(uint16_t durationMs) 
{
    _fadeState = In;
    _fadeStart = Millis();
    _fadeDurationMs = durationMs;
}

void StripEffect::fadeOut(uint16_t durationMs) 
{
    _fadeState = In;
    _fadeStart = Millis();
    _fadeDurationMs = durationMs;
}

void StripEffect::update()
{
    if (Millis() > _lastUpdateMs + _updatePeriodMs) {
        affect();
        _lastUpdateMs = Millis();
    }
}

uint16_t StripEffect::numLeds() 
{ 
    return sizeof(_ledData) / sizeof((_ledData)[0]); 
}

StripEffect::FadeState StripEffect::fadeState()
{
    return _fadeState;
}

