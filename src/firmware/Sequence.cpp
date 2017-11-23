#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "Sequence.h"
#include "Config.h"

Sequence::Sequence(CRGB* ledData, const TProgmemRGBPalette16& palette, bool smooth) :
    Effect(ledData),
    _palette(palette),
    _smooth(smooth),
    _lastColorChange(0),
    _chunkCounter(0),
    _hue(0)
{
    DBLN(F("Start Sequence"));
}

void Sequence::render()
{
    if (Millis() > _lastColorChange + ((128./SpeedFactor)*PeriodMs)) {
        _lastColorChange = Millis();
        if (_smooth) {
            _hue = _hue+1;
        } else {
            if (++_chunkCounter >= 16) {
                _chunkCounter = 0;
                _hue += 16;
            }
        }
        // modulusish
        while (_hue >= 255) { _hue -= 255; }

        // Shift all colors to the right
        for(uint16_t i=0; i<LedCount; i++) {
            _ledData[i] = ColorFromPalette(_palette, _hue);
        }
    }
}

