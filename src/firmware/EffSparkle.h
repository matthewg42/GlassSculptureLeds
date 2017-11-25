#pragma once

#include "Effect.h"

#define SPARKLE_SPEED_FACTOR (128./(SpeedFactor < 30 ? 30 : SpeedFactor))

/*! \class EffSparkle
 * LED Effect which just fades between the colors in a palette
 */
class EffSparkle : public Effect {
public:
    static const uint16_t UpdateMs = 20;
    static const uint16_t Darkening = 50;

public:
    EffSparkle(CRGB* ledData);
    void render();

protected:
    uint32_t _lastUpdate;

};

