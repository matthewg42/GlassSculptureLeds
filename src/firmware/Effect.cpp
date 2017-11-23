#include <Millis.h>
#include "Effect.h"
#include "Config.h"

Effect::Effect(CRGB* ledData) :
    _ledData(ledData)
{
    clearStrip();
}

Effect::~Effect() 
{
}

void Effect::clearStrip()
{
    for(uint16_t i=0; i<LedCount; i++) {
        _ledData[i] = CRGB::Black;
    }
}

