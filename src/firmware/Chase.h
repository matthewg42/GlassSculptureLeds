#pragma once

#include "StripEffect.h"

class Chase : public StripEffect {
public:
    Chase(CRGB* ledData, uint16_t numLeds, CRGB* colors, uint16_t numColors, uint16_t shiftPeriodMs=30, uint16_t colorPeriodMs=200);
    void render();

protected:
    CRGB* _colors;
    uint16_t _numColors;
    uint16_t _shiftPeriodMs;
    uint16_t _colorPeriodMs;
    uint32_t _lastShift;
    uint32_t _lastColorChange;
    uint16_t _colorIndex;

};

