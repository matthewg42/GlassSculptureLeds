#pragma once

#include "Effect.h"
#include "Config.h"

#define SPURT_SPEED_FACTOR (((SpeedFactor < 15 ? 15 : SpeedFactor)/128.))

struct SpurtBlob {
    CRGB color;
    float velocity;
    float location;

    SpurtBlob() : location(LedCount) {;}
    bool isDormant() { return location > LedCount-1 || location < 0; }

};

/*! \class EffSpurt
 * LED Moving spurts with different speeds
 */
class EffSpurt : public Effect {
public:
    static const uint8_t Count = 6;
    static const uint8_t TailLength = 30;
    static const uint16_t SpawnDelayMs = 600;
    
public:
    EffSpurt(CRGB* ledData, const TProgmemRGBPalette16& palette);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    SpurtBlob _spurts[Count];
    uint32_t _lastUpdate;
    uint32_t _lastSpawn;
    uint16_t _nextSpawnDelay;
    float _fadeCounter;

};

