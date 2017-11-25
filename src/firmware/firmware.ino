#include <math.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include <DebouncedButton.h>
#include <DiscretePot.h>
#include <MemoryFree.h>
#include <FastLED.h>
#include "Button.h"
#include "BrightnessFader.h"
#include "SpeedControl.h"
#include "Effect.h"
#include "EffSequence.h"
#include "EffChase.h"
#include "EffBlobs.h"
#include "EffSolid.h"
#include "Palettes.h"
#include "Config.h"

#define MEMFREE  do { DB(F("mem=")); DBLN(freeMemory()); } while (0)

// Global variables ////////////////////////////////////////////////////////////////////////////

#ifdef CROSSFADE
// Buffers of color data for rendering our effects 
CRGB Buffers[2][LedCount];              // Two buffers 
Effect* Effects[2] = { NULL, NULL };    // ...for two effects

typedef enum { None, JustA, JustB, AtoB, BtoA } t_BufferState;
t_BufferState BufferState = None;       // Keep track of transition state
uint32_t LastTransitionStart = 0;       // And when it started
uint8_t MixAmount = 128;                // 0: EffectA@100%, EffectB@0%; 255: EffectA@0%, EffectB@100%
CRGB LedData[LedCount];                 // Mapped onto our LED strip by FastLED lib
#else
// no crossfade - we render directly into the LedData buffer, so don't need lots
// of those globals. Save the RAM!
CRGB Buffers[1][LedCount];              // This will be rendered directly
Effect* Effects[1] = { NULL };          // and attached to this effect
#endif

uint8_t EffectIndex = 0;                // Used to keep track of which effect is next
uint32_t LastLedUpdate = 0;             // Used to control frame rate
uint8_t Brightness = 0;                 // For brightness control by user

// Functions ///////////////////////////////////////////////////////////////////////////////////

Effect* nextEffect(uint8_t buffer)
{
    Effect* effect = NULL;

#ifdef CROSSFADE
    if (EffectIndex==0) {
        effect = new EffSequence(Buffers[buffer], PaletteRed, true);
    } else if (EffectIndex==1) {
        effect = new EffBlobs(Buffers[buffer], ForestColors_p);
    } else if (EffectIndex==2) {
        effect = new EffChase(Buffers[buffer], PartyColors_p, true);
    } else {
        DB(F("nextEffect: invalid EffectIndex="));
        DBLN(EffectIndex);
        effect = NULL;
    }

    EffectIndex = (EffectIndex+1) % 3;
    return effect;
#else
    if (EffectIndex==0) {
        effect = new EffSequence(Buffers[buffer], PaletteRed, false);
    } else if (EffectIndex==1) {
        effect = new EffSequence(Buffers[buffer], PaletteBlue, false);
    } else if (EffectIndex==2) {
        effect = new EffSequence(Buffers[buffer], PaletteBlue, true);
    } else if (EffectIndex==3) {
        effect = new EffChase(Buffers[buffer], PaletteContrast, false);
    } else if (EffectIndex==4) {
        effect = new EffChase(Buffers[buffer], PaletteContrast, true);
    } else if (EffectIndex==5) {
        effect = new EffChase(Buffers[buffer], RainbowColors_p, true);
    } else if (EffectIndex==6) {
        effect = new EffBlobs(Buffers[buffer], PaletteContrast);
    } else if (EffectIndex==7) {
        effect = new EffBlobs(Buffers[buffer], PaletteGreen);
    } else if (EffectIndex==8) {
        effect = new EffBlobs(Buffers[buffer], CloudColors_p);
    } else {
        DB(F("nextEffect: invalid EffectIndex="));
        DBLN(EffectIndex);
        effect = NULL;
    }

    EffectIndex = (EffectIndex+1) % 9;
    return effect;
#endif
}

#ifdef CROSSFADE
void startCrossfade()
{
    switch(BufferState) {
    case JustA:
        DB(F("startCrossfade: starting AtoB "));
        MEMFREE;
        LastTransitionStart = Millis();
        BufferState = AtoB;
        if (Effects[1] != NULL) {
            delete Effects[1];
            Effects[1] = NULL;
        }
        Effects[1] = nextEffect(1);
        break;
    case JustB:
        DB(F("startCrossfade: starting BtoA "));
        MEMFREE;
        LastTransitionStart = Millis();
        BufferState = BtoA;
        if (Effects[0] != NULL) {
            delete Effects[0];
            Effects[0] = NULL;
        }
        Effects[0] = nextEffect(0);
        break;
    default:
        // DBLN(F("startCrossfade: already transitioning"));
        break;
    }
}

void updateTransition()
{
    float complete = (float)(Millis() - LastTransitionStart) / TransitionMs;
    switch(BufferState) {
    case AtoB:
        if (complete >= 1.0) {
            MixAmount = 255;
            BufferState = JustB;
            LastTransitionStart = 0;
            // DBLN(F("Transition AtoB complete"));
            if (Effects[0] != NULL) {
                delete Effects[0];
                Effects[0] = NULL;
            }
        } else {
            MixAmount = 255 * complete;
        }
        break;
    case BtoA:
        complete = (float)(Millis() - LastTransitionStart) / TransitionMs;
        if (complete >= 1.0) {
            MixAmount = 0;
            BufferState = JustA;
            LastTransitionStart = 0;
            // DBLN(F("Transition BtoA complete"));
            if (Effects[1] != NULL) {
                delete Effects[1];
                Effects[1] = NULL;
            }
        } else {
            MixAmount = 255 - (255 * complete);
        }
        break;
    default:
        // nothing to do
        break;
    }
}

void mixAdd(CRGB* src, CRGB* dest, uint16_t count, uint8_t scale=255) 
{
    for(uint16_t i=0; i<count; i++) {
        dest[i] = dest[i].lerp8(src[i], scale);
    }
}

#else  // end CROSSFADE
void switchEffect()
{
    // for switchEffect, we just just slot A, deleting any existing effect before 
    // starting the next one
    DB(F("switchEffect: next!"));
    if (Effects[0] != NULL) {
        delete Effects[0];
        Effects[0] = NULL;
    }
    Effects[0] = nextEffect(0);
}
#endif // end !CROSSFADE

void ledClear(CRGB* dest, uint16_t count) 
{
    for(uint16_t i=0; i<count; i++) {
        dest[i] = CRGB::Black;
    }
}

void ledUpdate()
{
    if (LastLedUpdate == 0 || Millis() > LastLedUpdate + LedRefreshMs) {
        FastLED.show();
        addMillisOffset(LedCount*0.0215);
        LastLedUpdate = Millis();
    }
}

void setup()
{
    Serial.begin(115200);

    // Init h/w
    Button.begin();
    BrightnessFader.begin(1, 64, true);
    SpeedControl.begin(1, 64, true);

#ifdef CROSSFADE
    FastLED.addLeds<LedChipset, LedPin, LedOrder>(LedData, LedCount);
    BufferState = JustA;
    MixAmount = 0;
#else
    FastLED.addLeds<LedChipset, LedPin, LedOrder>(Buffers[0], LedCount);
#endif

    // Initial effect state
    Effects[0] = nextEffect(0);
    
    // The analog pins need time to settle.  While this is happening, I call
    // Update on the DiscretePot objects to stabalize their value
    while(Millis() < (500)) {
        BrightnessFader.update();
        SpeedControl.update();
    }

    MEMFREE;
    DBLN(F("E:setup"));
}

void loop()
{
    Button.update();
    BrightnessFader.update();
    SpeedControl.update();

    uint8_t speed8 = (SpeedControl.value()*4)-1;
    if (speed8 != SpeedFactor) {
        SpeedFactor = speed8;
        DB(F("Speed="));
        DB(SpeedFactor);
        DB(' ');
        MEMFREE;
    }

    if (Button.tapped()) {
#ifdef CROSSFADE
        startCrossfade();
#else
        switchEffect();
#endif
    }

#ifdef CROSSFADE
    updateTransition();
#endif

    if (BrightnessFader.value() != Brightness) {
        DB(F("Brightness level="));
        DB(BrightnessFader.value());
        DB(F(" scale8="));
        DB((BrightnessFader.value()*4)-1);
        DB(' ');
#ifdef FIXEDBRIGHTNESS
        FastLED.setBrightness(FIXEDBRIGHTNESS);
#else
        FastLED.setBrightness((BrightnessFader.value()*4)-1);
#endif
        Brightness = BrightnessFader.value();
        MEMFREE;
    }

#ifdef CROSSFADE
    // Render it all out
    ledClear(LedData, LedCount);
    if (Effects[0]) { 
        Effects[0]->render(); 
        mixAdd(Buffers[0], LedData, LedCount, 255-MixAmount);
    }
    if (Effects[1]) { 
        Effects[1]->render(); 
        mixAdd(Buffers[1], LedData, LedCount, MixAmount);
    }
#else
    if (Effects[0]) { 
        Effects[0]->render(); 
    }
#endif
    ledUpdate();
}

