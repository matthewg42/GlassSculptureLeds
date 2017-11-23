#pragma once

#include <FastLED.h>
#include "Effect.h"

/*! \class Chase
 * LED Effect which sends colors along the strip in a fixed sequence
 */
class Chase : public Effect {
public:
    static const uint16_t ShiftPeriodMs = 30;
    static const uint16_t ColorPeriodMs = 100;

public:
    Chase(CRGB* ledData, const TProgmemRGBPalette16& palette);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    uint32_t _lastShift;
    uint32_t _lastColorChange;
    uint8_t _colorIndex;

};

