#include <Millis.h>
#include <MutilaDebug.h>
#include "SpeedControl.h"
#include "EffSparkle.h"
#include "Config.h"

EffSparkle::EffSparkle(CRGB* ledData) :
    Effect(ledData),
    _lastUpdate(0)
{
    DBLN(F("Start EffSparkle"));
}

void EffSparkle::render()
{
    if (Millis() < _lastUpdate + SPARKLE_SPEED_FACTOR*UpdateMs) {
        return;
    }

    uint16_t i = random(0, LedCount);
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    switch (random(15+Darkening)) {
    case 0:
        g = 220;
        break;
    case 1:
        r = 220;
        break;
    case 2:
    case 3:
    case 4:
        r = 80;
        g = 128;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        r = 30;
        g = 70;
        b = 205;
        break;
    default:
        r = 0;
        g = 0;
        b = 0;
    }

    _ledData[i].setRGB(r, g, b);
    r /= 3;
    g /= 3;
    b /= 3;
    if (i > 0) _ledData[i-1].setRGB(r, g, b);
    if (i < LedCount-1) _ledData[i+1].setRGB(r, g, b);

    for(i=0; i<LedCount; i++) {
        _ledData[i].subtractFromRGB(1);
    }

    _lastUpdate = Millis();
}

