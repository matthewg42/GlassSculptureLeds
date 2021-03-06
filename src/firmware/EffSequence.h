#pragma once

#include "Effect.h"

#define SEQUENCE_SPEED_FACTOR (128./(SpeedFactor < 25 ? 25 : SpeedFactor))

/*! \class EffSequence
 * LED Effect which just fades between the colors in a palette
 */
class EffSequence : public Effect {
public:
    static const uint16_t PeriodMs = 25;

public:
    EffSequence(CRGB* ledData, const TProgmemRGBPalette16& palette, bool smooth=true);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    bool _smooth;
    uint32_t _lastColorChange;
    uint8_t _chunkCounter;
    float _hue;
    bool _hasDisplayed;

};

