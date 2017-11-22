#pragma once

#include "StripEffect.h"

struct Blob {
    uint16_t firstPixel;        //!< Where on the strip we will start to draw this blob
    uint16_t fadePixels;        //!< How many pixels to fade in and out
    uint16_t onPixels;          //!< How broad the brightnest part is (in pixels)
    uint16_t fadeMs;            //!< How many milliseconds we will take to fade in and out
    uint16_t onMs;              //!< How long we will remain full on
    uint32_t birthMs;           //!< At what time was this blob born? (0 means dormant)

    Blob() : birthMs(0) {;}

};

/*! \class Blobs
 * LED StripEffect which makes blobs fade in and out at random places along the strip.
 * The edges of the blobs are faded to black, and they wrap around the ends of the strip.
 * Blob colors are chosen randomly from the color scheme provided
 */
class Blobs : public StripEffect {
public:
    static const uint8_t NumberOfBlobs = 5;
    static const uint16_t MinSpawnDelayMs = 500;

public:
    Blobs(CRGB* ledData, uint16_t numLeds, CRGB* colors, uint16_t numColors);
    void render();
    void spawn();

protected:
    CRGB* _colors;
    uint16_t _numColors;
    Blob _blobs[NumberOfBlobs];
    uint32_t _lastSpawn;

};

