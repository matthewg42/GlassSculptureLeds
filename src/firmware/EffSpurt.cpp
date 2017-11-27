#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "EffSpurt.h"
#include "Config.h"

EffSpurt::EffSpurt(CRGB* ledData, const TProgmemRGBPalette16& palette) :
    Effect(ledData),
    _palette(palette),
    _lastSpawn(0),
    _fadeCounter(0)
{
    DBLN(F("Start EffSpurt"));
}

void EffSpurt::render()
{
    // Fade existing pixels
    _fadeCounter += SPURT_SPEED_FACTOR/30;
    if (_fadeCounter > 30) {
        for (uint16_t i=0; i<LedCount; i++) {
            // _ledData[i].subtractFromRGB(1);
            _ledData[i].fadeToBlackBy(1);
        }
    }

    // For each non-dormant spurt, advance it and draw the head
    for (uint8_t i=0; i<Count; i++) {
        if (!_spurts[i].isDormant()) {
            _ledData[(uint16_t)_spurts[i].location] += _spurts[i].color;
            _spurts[i].location += _spurts[i].velocity / SPURT_SPEED_FACTOR;
        } else if (Millis() > _lastSpawn + SpawnDelayMs) {
            _lastSpawn = Millis();
            _spurts[i].location = 0.0;
            _spurts[i].color = ColorFromPalette(_palette, random(256));
            _spurts[i].velocity = random(10,100) / 5000.;
        }
    }
}

