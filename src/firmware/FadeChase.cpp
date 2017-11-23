#include <Millis.h>
#include <MutilaDebug.h>
#include "FadeChase.h"

FadeChase::FadeChase(CRGB* ledData, const uint16_t numLeds, const CRGB* colors, const uint16_t numColors, const uint16_t shiftPeriodMs, const uint16_t colorPeriodMs) :
    StripEffect(ledData, numLeds),
    _colors(colors), 
    _numColors(numColors),
    _shiftPeriodMs(shiftPeriodMs),
    _colorPeriodMs(colorPeriodMs),
    _lastShift(0),
    _lastColorChange(0),
    _colorIndex(0)
{
    DBLN(F("Start FadeChase"));
}

void FadeChase::render()
{
    if (Millis() > _lastColorChange + _colorPeriodMs) {
        _lastColorChange = Millis();
        _colorIndex = (_colorIndex + 1) % _numColors;
    }

    if (Millis() > _lastShift + _shiftPeriodMs) {
        _lastShift = Millis();

        // Shift all colors to the right
        for(uint16_t i=_numLeds-1; i>0; i--) {
            _ledData[i] = _ledData[i-1];
        }

        uint16_t nextColorIndex = (_colorIndex + 1) % _numColors;
        uint16_t msSinceLastChange = Millis() - _lastColorChange;
        float fade = (float)msSinceLastChange / _colorPeriodMs;
        if (fade > 1) fade = 1;

        // Put the current color at the start
        _ledData[0] = _colors[_colorIndex].lerp16(_colors[nextColorIndex], 65535*fade);
    }
}

