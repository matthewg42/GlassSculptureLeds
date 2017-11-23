#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "EffSolid.h"
#include "Config.h"

EffSolid::EffSolid(CRGB* ledData, const CRGB color) :
    Effect(ledData),
    _color(color)
{
    DBLN(F("Start EffSolid"));
    for (uint16_t i=0; i<LedCount; i++) {
        _ledData[i] = color;
    }
}

void EffSolid::render()
{
}

