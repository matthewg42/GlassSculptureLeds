#include <Millis.h>
#include <MutilaDebug.h>
#include "Blobs.h"

Blobs::Blobs(CRGB* ledData, uint16_t numLeds, CRGB* colors, uint16_t numColors) :
    StripEffect(ledData, numLeds),
    _colors(colors), 
    _numColors(numColors),
    _lastSpawn(0)
{
}

void Blobs::render()
{
    if (Millis() > _lastSpawn + MinSpawnDelayMs || _lastSpawn == 0) {
        spawn();
    }

    for (uint8_t i=0; i<NumberOfBlobs; i++) {
        // TODO draw it
    }
}

void Blobs::spawn()
{
    for (uint8_t i=0; i<NumberOfBlobs; i++) {
        if (_blobs[i].birthMs == 0) {
            DB(F("Blobs::render spawning in slot "));
            DBLN(i);
            _lastSpawn = Millis();
            _blobs[i].birthMs = Millis()+1;             // hack to prevent suppression at Millis() == 0
            _blobs[i].firstPixel = random(_numLeds-1);
            _blobs[i].fadePixels = random(1,6);
            _blobs[i].onPixels = random(1,6);
            _blobs[i].fadeMs = random(500, 5001);
            _blobs[i].onMs = random(500, 5001);
            return;
        }
    }
}


