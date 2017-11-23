#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "Chase.h"
#include "Config.h"

Chase::Chase(CRGB* ledData, const TProgmemRGBPalette16& palette, bool smooth) :
    Effect(ledData),
    _palette(palette), 
    _smooth(smooth),
    _lastShift(0),
    _hue(0.)
{
    DBLN(F("Start Chase"));
}

void Chase::render()
{
    if (Millis() > _lastShift + ((128./SpeedFactor)*ShiftPeriodMs)) {
        _lastShift = Millis();

        // See if it's time to update the color...
        if (!_smooth) {
            if (_pixelCount++ >= ColorLengthPixels) {
                _pixelCount = 0;
                _hue = _hue + 16;
            }
        } else {
            // every step we increment... how much depends on ColorLengthPixels
            // the goal is that in ColorLengthPixels steps we will increment 16
            _hue = (_hue + (16./ColorLengthPixels));
        }
        // modulusish
        while (_hue >= 255) { _hue -= 255; }

        // Shift all colors to the right
        for(uint16_t i=LedCount-1; i>0; i--) {
            _ledData[i] = _ledData[i-1];
        }

        // Put the current color at the start
        _ledData[0] = ColorFromPalette(_palette, _hue);
    }
}

