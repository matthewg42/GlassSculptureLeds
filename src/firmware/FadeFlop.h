#pragma once

#include "StripEffect.h"

/*! \class FadeFlop
 * LED StripEffect which just fades between some colors
 */
class FadeFlop : public StripEffect {
public:
    FadeFlop(CRGB* ledData, uint16_t numLeds, uint16_t periodMs, CRGB* colors, uint16_t numColors);
    void render();

protected:
    const CRGB* _colors;
    uint16_t _numColors;
    uint16_t _periodMs;
    uint32_t _lastColorChange;
    uint16_t _colorIndex;

};

