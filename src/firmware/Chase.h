#pragma once

#include "StripEffect.h"

/*! \class Chase
 * LED StripEffect which sends colors along the strip in a fixed sequence
 */
class Chase : public StripEffect {
public:
    Chase(CRGB* ledData, const uint16_t numLeds, const CRGB* colors, 
          const uint16_t numColors, const uint16_t shiftPeriodMs=30, 
          const uint16_t colorPeriodMs=200);
    void render();

protected:
    const CRGB* _colors;
    uint16_t _numColors;
    uint16_t _shiftPeriodMs;
    uint16_t _colorPeriodMs;
    uint32_t _lastShift;
    uint32_t _lastColorChange;
    uint16_t _colorIndex;

};

