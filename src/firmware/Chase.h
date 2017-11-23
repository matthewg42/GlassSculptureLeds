#pragma once

#include <FastLED.h>
#include "Effect.h"

/*! \class Chase
 * LED Effect which sends colors along the strip in a fixed sequence
 */
class Chase : public Effect {
public:
    static const uint16_t ShiftPeriodMs = 30;
    static const uint8_t ColorLengthPixels = 10;

public:
    Chase(CRGB* ledData, const TProgmemRGBPalette16& palette, bool smooth=true);
    void render();

protected:
    const TProgmemRGBPalette16& _palette;
    bool _smooth;
    uint32_t _lastShift;
    uint8_t _pixelCount;
    float _hue;

};

