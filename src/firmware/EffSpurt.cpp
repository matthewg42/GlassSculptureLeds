#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "EffSpurt.h"
#include "Config.h"

EffSpurt::EffSpurt(CRGB* ledData, const TProgmemRGBPalette16& palette) :
    Effect(ledData),
    _palette(palette),
    _lastSpawn(0),
    _nextSpawnDelay(0),
    _fadeCounter(0)
{
    DBLN(F("Start EffSpurt"));
}

void EffSpurt::render()
{
    // Fade existing pixels
    _fadeCounter += SPURT_SPEED_FACTOR;
    if (_fadeCounter > TailLength) {
        for (uint16_t i=0; i<LedCount; i++) {
            _ledData[i].fadeToBlackBy(1);
        }
    }

    // For each non-dormant spurt, advance it and draw the head
    for (uint8_t i=0; i<Count; i++) {
        if (!_spurts[i].isDormant()) {
            _ledData[(uint16_t)_spurts[i].location] += _spurts[i].color;
            _spurts[i].location += SPURT_SPEED_FACTOR * _spurts[i].velocity;
        } else if (MillisSince(_lastSpawn) > SPURT_SPEED_FACTOR*_nextSpawnDelay) {
            DB(Millis());
            DBLN(F(" Spurt Spawn"));
            _nextSpawnDelay = random(50,1200) / SPURT_SPEED_FACTOR;
            _spurts[i].location = 0;
            _spurts[i].color = ColorFromPalette(_palette, random(256));
            _spurts[i].velocity = random(20,200) / 1600.;
        }
    }
}

