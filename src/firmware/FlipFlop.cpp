#include <Millis.h>
#include <MutilaDebug.h>
#include "FlipFlop.h"

FlipFlop::FlipFlop(CRGB* ledData, uint16_t numLeds, uint16_t periodMs, CRGB* colors, uint16_t numColors) :
    StripEffect(ledData, numLeds),
    _colors(colors), 
    _numColors(numColors),
    _periodMs(periodMs),
    _flipIndex(0),
    _lastFlip(0)
{
}

void FlipFlop::render()
{
    if (Millis() > _lastFlip + _periodMs) {
        _lastFlip = Millis();
        _flipIndex = (_flipIndex + 1) % _numColors;
    }

    for(uint16_t i=0; i<_numLeds; i++)
    {
        _ledData[i] = _colors[_flipIndex];
    }
}

