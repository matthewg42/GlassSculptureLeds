#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "EffSequence.h"
#include "Config.h"

EffSequence::EffSequence(CRGB* ledData, const TProgmemRGBPalette16& palette, bool smooth) :
    Effect(ledData),
    _palette(palette),
    _smooth(smooth),
    _lastColorChange(0),
    _chunkCounter(0),
    _hue(0)
{
    DBLN(F("Start EffSequence"));
}

void EffSequence::render()
{
    if (Millis() > _lastColorChange + (SEQUENCE_SPEED_FACTOR*PeriodMs)) {
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

