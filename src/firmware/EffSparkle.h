#pragma once

#include "Effect.h"

#define SPARKLE_SPEED_FACTOR ((0.01*(128./SpeedFactor))*(0.01*(128./SpeedFactor))*(0.01*(128./SpeedFactor)))

/*! \class EffSparkle
 * LED Effect which just fades between the colors in a palette
 */
class EffSparkle : public Effect {
public:
    static const uint16_t UpdateMs = 20;
    static const uint16_t SparkleAmount = 100;

public:
    EffSparkle(CRGB* ledData, const TProgmemRGBPalette16& palette);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    uint32_t _lastUpdate;
    uint8_t _fadeCounter;

};

