#pragma once

#include "Effect.h"
#include "Config.h"

#define SPURT_SPEED_FACTOR (0.5*(128./(SpeedFactor < 30 ? 30 : SpeedFactor)))

struct SpurtBlob {
    CRGB color;
    float velocity;
    float location;

    SpurtBlob() : location(LedCount) {;}
    bool isDormant() { return location > LedCount-1; }

};

/*! \class EffSpurt
 * LED Moving spurts with different speeds
 */
class EffSpurt : public Effect {
public:
    static const uint8_t Count = 3;
    static const uint16_t SpawnDelayMs = 200;
    
public:
    EffSpurt(CRGB* ledData, const TProgmemRGBPalette16& palette);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    SpurtBlob _spurts[Count];
    uint32_t _lastSpawn;
    float _fadeCounter;

};

