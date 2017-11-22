#include <Millis.h>
#include <MutilaDebug.h>
#include "Chase.h"

Chase::Chase(CRGB* ledData, uint16_t numLeds, CRGB* colors, uint16_t numColors, uint16_t shiftPeriodMs, uint16_t colorPeriodMs) :
    StripEffect(ledData, numLeds),
    _colors(colors), 
    _numColors(numColors),
    _shiftPeriodMs(shiftPeriodMs),
    _colorPeriodMs(colorPeriodMs),
    _lastShift(0),
    _lastColorChange(0),
    _colorIndex(0)
{
}

void Chase::render()
{
    if (Millis() > _lastColorChange + _colorPeriodMs) {
        _lastColorChange = Millis();
        _colorIndex = (_colorIndex + 1) % _numColors;
        DB(F("Chase::render color="));
        DBLN(_colorIndex);
    }

    if (Millis() > _lastShift + _shiftPeriodMs) {
        _lastShift = Millis();

        // Shift all colors to the right
        for(uint16_t i=_numLeds-1; i>0; i--) {
            _ledData[i] = _ledData[i-1];
        }

        // Put the current color at the start
        _ledData[0] = _colors[_colorIndex];
    }
}

