#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "EffSparkle.h"
#include "Config.h"

EffSparkle::EffSparkle(CRGB* ledData, const TProgmemRGBPalette16& palette) :
    Effect(ledData),
    _palette(palette),
    _lastUpdate(0),
    _fadeCounter(0)
{
    DBLN(F("Start EffSparkle"));
}

void EffSparkle::render()
{
    DB(F("sparkle speed: "));
    DBLN(SPARKLE_SPEED_FACTOR);
    if (Millis() < _lastUpdate + (UpdateMs/SPARKLE_SPEED_FACTOR)) {
        return;
    }

    // Dim existing leds
    if (_fadeCounter++ > 3) {
        _fadeCounter = 0;
        for(uint16_t i=0; i<LedCount; i++) {
            _ledData[i].fadeToBlackBy(20);
        }
    }
    
    if (random(LedCount*LedCount/SparkleAmount) == 0) {
        // new sparkle
        uint16_t i = random(LedCount);
        CRGB color = ColorFromPalette(_palette, random(256));
        _ledData[i] = color;
        if (i > 0) _ledData[i-1] = color.scale8(64);
        if (i < LedCount-1) _ledData[i+1] = color.scale8(128);
    }

    _lastUpdate = Millis();
}

