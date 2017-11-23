#pragma once

#include "StripEffect.h"

struct Blob {
    uint16_t firstPixel;        //!< Where on the strip we will start to draw this blob
    uint16_t fadePixels;        //!< How many pixels to fade in and out
    uint16_t onPixels;          //!< How broad the brightnest part is (in pixels)
    uint16_t fadeMs;            //!< How many milliseconds we will take to fade in and out
    uint16_t onMs;              //!< How long we will remain full on
    uint32_t birthMs;           //!< At what time was this blob born? (0 means dormant)
    CRGB color;                 //!< The color of the blob at max brightness

    Blob() : birthMs(0), color(CRGB::White) {;}
    bool isDormant() { return birthMs == 0 || Millis() > birthMs+fadeMs+fadeMs+onMs; }
    uint32_t ageMs() { return isDormant() ? 0 : Millis() - birthMs; }

};

/*! \class Blobs
 * LED StripEffect which makes blobs fade in and out at random places along the strip.
 * The edges of the blobs are faded to black, and they wrap around the ends of the strip.
 * Blob colors are chosen randomly from the color scheme provided
 */
class Blobs : public StripEffect {
public:
    static const uint8_t NumberOfBlobs = 3;
    static const uint16_t MinSpawnDelayMs = 500;

public:
    Blobs(CRGB* ledData, const uint16_t numLeds, const CRGB* colors, const uint16_t numColors);
    void render();
    void spawn();
    void renderBlob(Blob& blob);

protected:
    const CRGB* _colors;
    uint16_t _numColors;
    Blob _blobs[NumberOfBlobs];
    uint32_t _lastSpawn;

};

