#pragma once

#include "Effect.h"

/*! \class FadeFlop
 * LED Effect which just fades between some colors
 */
class FadeFlop : public Effect {
public:
    static const uint16_t PeriodMs = 1000;

public:
    FadeFlop(CRGB* ledData, const TProgmemRGBPalette16& palette);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    uint32_t _lastColorChange;
    uint16_t _colorIndex;

};

