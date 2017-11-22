#include <Millis.h>
#include "StripEffect.h"

StripEffect::StripEffect(CRGB* ledData, uint16_t numLeds) :
    _ledData(ledData),
    _numLeds(numLeds)
{
}

StripEffect::~StripEffect() 
{
}

