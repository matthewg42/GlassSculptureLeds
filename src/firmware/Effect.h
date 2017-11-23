#pragma once

#include <FastLED.h>

class Effect {
public:
    /*! Constructor */
    Effect(CRGB* ledData);
    /*! Destructor */
    virtual ~Effect();
    /*! Render the effect */
    virtual void render() = 0;
    /*! Clear the LEDs in the strip (set to black) */
    void clearStrip(); 

protected:
    CRGB* _ledData;

};

