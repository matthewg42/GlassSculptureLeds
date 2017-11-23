#include <Millis.h>
#include <MutilaDebug.h>
#include "FlipFlop.h"

FlipFlop::FlipFlop(CRGB* ledData, uint16_t numLeds, uint16_t periodMs, CRGB* colors, uint16_t numColors) :
    StripEffect(ledData, numLeds),
    _colors(colors), 
    _numColors(numColors),
    _periodMs(periodMs),
    _lastColorChange(0),
    _colorIndex(0)
{
    DBLN(F("Start FlipFlop"));
}

void FlipFlop::render()
{
    if (Millis() > _lastColorChange + _periodMs) {
        _lastColorChange = Millis();
        _colorIndex = (_colorIndex + 1) % _numColors;
    }

    for(uint16_t i=0; i<_numLeds; i++)
    {
        _ledData[i] = _colors[_colorIndex];
    }
}

