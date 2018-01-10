#pragma once

#include "Effect.h"

#define SPARKLE_SPEED_FACTOR ((((float)(SpeedFactor < 20 ? 20 : SpeedFactor))*0.5)/255)

/*! \class EffSparkle
 * LED Effect which just fades between the colors in a palette
 */
class EffSparkle : public Effect {
public:
    static const uint16_t UpdateMs = 3;
    static const uint16_t SparkleAmount = 5000;

public:
    EffSparkle(CRGB* ledData, const TProgmemRGBPalette16& palette);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    uint32_t _lastUpdate;
    uint8_t _fadeCounter;

};

