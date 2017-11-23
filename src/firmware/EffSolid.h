#pragma once

#include "Effect.h"

/*! \class EffSolid
 * LED Effect which just fades between the colors in a palette
 */
class EffSolid : public Effect {
public:
    static const uint16_t PeriodMs = 100;

public:
    EffSolid(CRGB* ledData, const CRGB color);
    void render();

protected:
    const CRGB _color;

};

