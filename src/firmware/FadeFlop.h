#pragma once

#include "StripEffect.h"

/*! \class FadeFlop
 * LED StripEffect which just fades between some colors
 */
class FadeFlop : public StripEffect {
public:
    FadeFlop(CRGB* ledData, const uint16_t numLeds, const uint16_t periodMs, const CRGB* colors, const uint16_t numColors);
    void render();

protected:
    const CRGB* _colors;
    uint16_t _numColors;
    uint16_t _periodMs;
    uint32_t _lastColorChange;
    uint16_t _colorIndex;

};

