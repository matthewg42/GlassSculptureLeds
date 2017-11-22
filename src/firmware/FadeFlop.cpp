#include <Millis.h>
#include <MutilaDebug.h>
#include "FadeFlop.h"

FadeFlop::FadeFlop(CRGB* ledData, uint16_t numLeds, uint16_t periodMs, CRGB* colors, uint16_t numColors) :
    StripEffect(ledData, numLeds),
    _colors(colors), 
    _numColors(numColors),
    _periodMs(periodMs),
    _lastColorChange(0),
    _colorIndex(0)
{
}

void FadeFlop::render()
{
    if (Millis() > _lastColorChange + _periodMs) {
        _lastColorChange = Millis();
        _colorIndex = (_colorIndex + 1) % _numColors;
    }

    uint16_t nextColorIndex = (_colorIndex + 1) % _numColors;
    uint16_t msSinceLastChange = Millis() - _lastColorChange;
    float fade = (float)msSinceLastChange / _periodMs;
    if (fade > 1) fade = 1;
    for(uint16_t i=0; i<_numLeds; i++)
    {
        _ledData[i] = _colors[_colorIndex].lerp16(_colors[nextColorIndex], 65535*fade);
    }
}

