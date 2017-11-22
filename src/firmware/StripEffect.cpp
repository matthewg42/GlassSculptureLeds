#include <Millis.h>
#include "StripEffect.h"

StripEffect::StripEffect(CRGB* ledData, uint16_t numLeds) :
    _ledData(ledData),
    _numLeds(numLeds),
    _fadeState(On),
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
    _fadeState = Out;
    _fadeStart = Millis();
    _fadeDurationMs = durationMs;
}

float StripEffect::fadeRatio()
{
    uint32_t since = Millis() - _fadeStart;
    switch(_fadeState) {
    case In:
        if (since >= _fadeDurationMs) { 
            return 1.0; 
        } else {
            return ((float)since) / _fadeDurationMs;
        }
        break;
    case On:
        return 1.0;
        break;
    case Out:
        if (since >= _fadeDurationMs) { 
            return 0.0; 
        } else {
            return 1.0 - (((float)since) / _fadeDurationMs);
        }
        break;
    case Off:
        return 0.0;
        break;
    default:
        return 1.0;
    }
}

StripEffect::FadeState StripEffect::fadeState()
{
    return _fadeState;
}

