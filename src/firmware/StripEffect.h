#pragma once

#include <FastLED.h>

class StripEffect {
public:
    /*! Constructor */
    StripEffect(CRGB* ledData, uint16_t numLeds);
    /*! Destructor */
    virtual ~StripEffect();
    /*! Render the effect */
    virtual void render() = 0;
    /*! Trigger fade in of effect */

protected:
    CRGB* _ledData;
    uint16_t _numLeds;

};

