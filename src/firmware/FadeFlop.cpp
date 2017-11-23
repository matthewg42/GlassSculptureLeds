#include <Millis.h>
#include <MutilaDebug.h>
#include "FadeFlop.h"
#include "Config.h"

FadeFlop::FadeFlop(CRGB* ledData, const TProgmemRGBPalette16& palette) :
    Effect(ledData),
    _palette(palette),
    _lastColorChange(0),
    _colorIndex(0)
{
    DBLN(F("Start FadeFlop"));
}

void FadeFlop::render()
{
    if (Millis() > _lastColorChange + PeriodMs) {
        _lastColorChange = Millis();
        _colorIndex = (_colorIndex + 1) % 16;
    }

    uint16_t nextColorIndex = (_colorIndex + 1) % 16;
    uint16_t msSinceLastChange = Millis() - _lastColorChange;
    float fade = (float)msSinceLastChange / PeriodMs;
    if (fade > 1) fade = 1;
    for(uint16_t i=0; i<LedCount; i++) {
        _ledData[i] = ColorFromPalette(_palette, _colorIndex).lerp8(ColorFromPalette(_palette, nextColorIndex), 255*fade);
    }
}

