#include <Millis.h>
#include <MutilaDebug.h>
#include "EffBlobs.h"
#include "Config.h"

EffBlobs::EffBlobs(CRGB* ledData, const TProgmemRGBPalette16& palette) :
    Effect(ledData),
    _palette(palette),
    _lastSpawn(0)
{
    DBLN(F("Start EffBlobs"));
}

void EffBlobs::render()
{
    if (Millis() > _lastSpawn + MinSpawnDelayMs || _lastSpawn == 0) {
        spawn();
    }

    // clear
    for (uint16_t c=0; c<LedCount; c++) {
        _ledData[c] = CRGB::Black;
    }
    
    // render blobs in additive way
    for (uint8_t i=0; i<NumberOfEffBlobs; i++) {
        renderBlob(_blobs[i]);
    }
}

void EffBlobs::renderBlob(Blob& blob)
{
    if (!blob.isDormant()) {
        float brightness = 0.;
        uint32_t age = blob.ageMs();
        if (age < blob.fadeMs) {
            // calculate brightness as fraction of fadeMs passed so far
            brightness = (float)age / blob.fadeMs;
        } else if (age < blob.fadeMs + blob.onMs) {
            // full on
            brightness = 1.;
        } else if (age < (2*blob.fadeMs) + blob.onMs) {
            brightness = 1-((float)(age - blob.onMs - blob.fadeMs) / blob.fadeMs);
        }

        uint16_t pos = blob.firstPixel;
        // draw fade in
        for (uint16_t i=1; i<=blob.fadePixels; i++) {
            float rampBright = (float)i / blob.fadePixels;
            _ledData[pos] += blob.color.scale8(CRGB(brightness*rampBright*255, brightness*rampBright*255, brightness*rampBright*255));
            pos = (pos + 1) % LedCount;
        }
        // draw full section
        for (uint16_t i=0; i<blob.onPixels; i++) {
            _ledData[pos] += blob.color.scale8(CRGB(brightness*255,brightness*255,brightness*255));
            pos = (pos + 1) % LedCount;
        }
        // draw fade out
        for (uint16_t i=blob.fadePixels; i>0; i--) {
            float rampBright = (float)i / blob.fadePixels;
            _ledData[pos] += blob.color.scale8(CRGB(brightness*rampBright*255, brightness*rampBright*255, brightness*rampBright*255));
            pos = (pos + 1) % LedCount;
        }
    }
}

void EffBlobs::spawn()
{
    for (uint8_t i=0; i<NumberOfEffBlobs; i++) {
        if (_blobs[i].isDormant()) {
            _lastSpawn = Millis();
            _blobs[i].birthMs = Millis()+1;             // hack to prevent suppression at Millis() == 0
            _blobs[i].firstPixel = random(LedCount-1);
            _blobs[i].fadePixels = random(1,6);
            _blobs[i].onPixels = random(1,6);
            _blobs[i].fadeMs = random(100, 3001);
            _blobs[i].onMs = random(500, 2001);
            _blobs[i].color = ColorFromPalette(_palette,random(16));
            return;
        }
    }
}


