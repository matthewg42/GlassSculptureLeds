#pragma once

#include "StripEffect.h"

class FlipFlop : public StripEffect {
public:
    FlipFlop(CRGB* ledData, uint16_t numLeds, uint16_t periodMs, CRGB* colors, uint16_t numColors);
    void render();

protected:
    CRGB* _colors;
    uint16_t _numColors;
    uint16_t _periodMs;
    uint32_t _lastColorChange;
    uint16_t _colorIndex;

};

