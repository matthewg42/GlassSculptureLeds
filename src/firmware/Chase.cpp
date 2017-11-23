#include <Millis.h>
#include <MutilaDebug.h>
#include "Chase.h"
#include "Config.h"

Chase::Chase(CRGB* ledData, const TProgmemRGBPalette16& palette) :
    Effect(ledData),
    _palette(palette), 
    _lastShift(0),
    _hue(0)
{
    DBLN(F("Start Chase"));
}

void Chase::render()
{
    if (_pixelCount >= ColorLengthPixels) {
        _pixelCount = 0;
        _hue = (_hue + 16) % 256;
    }

    if (Millis() > _lastShift + ShiftPeriodMs) {
        _lastShift = Millis();

        // Shift all colors to the right
        for(uint16_t i=LedCount-1; i>0; i--) {
            _ledData[i] = _ledData[i-1];
        }

        // Put the current color at the start
        _ledData[0] = ColorFromPalette(_palette, _hue);
        _pixelCount++;
    }
}

