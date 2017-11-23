#pragma once

#include <FastLED.h>

class Effect {
public:
    /*! Constructor */
    Effect(CRGB* ledData, uint16_t numLeds);
    /*! Destructor */
    virtual ~Effect();
    /*! Render the effect */
    virtual void render() = 0;
    /*! Trigger fade in of effect */

protected:
    CRGB* _ledData;
    uint16_t _numLeds;

};

