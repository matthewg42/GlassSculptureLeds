#include <Millis.h>
#include "Effect.h"

Effect::Effect(CRGB* ledData, uint16_t numLeds) :
    _ledData(ledData),
    _numLeds(numLeds)
{
}

Effect::~Effect() 
{
}

