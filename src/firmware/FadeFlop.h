#pragma once

#include "Effect.h"

/*! \class FadeFlop
 * LED Effect which just fades between the colors in a palette
 */
class FadeFlop : public Effect {
public:
    static const uint16_t PeriodMs = 10;

public:
    FadeFlop(CRGB* ledData, const TProgmemRGBPalette16& palette, bool smooth=true);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    bool _smooth;
    uint32_t _lastColorChange;
    float _hue;

};

