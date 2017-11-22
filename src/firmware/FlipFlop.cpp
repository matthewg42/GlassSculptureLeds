#include <Millis.h>
#include <MutilaDebug.h>
#include "FlipFlop.h"

FlipFlop::FlipFlop(CRGB* ledData, uint16_t numLeds, uint16_t periodMs, CRGB* colors, uint16_t numColors) :
    StripEffect(ledData, numLeds, periodMs),
    _colors(colors), 
    _numColors(numColors),
    _flipIndex(0)
{
}

void FlipFlop::affect()
{
    DB(F("FlipFlop::affect, leds="));
    DB(_numLeds);
    DB(F(" colors="));
    DBLN(_numColors);
    
    for(uint16_t i=0; i<_numLeds; i++)
    {
        _ledData[i] = _colors[_flipIndex];
    }
    _flipIndex = (_flipIndex + 1) % _numColors;
}

