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
#include "Effect.h"
#include "FadeFlop.h"
#include "Chase.h"
#include "Blobs.h"
#include "Palettes.h"
#include "Config.h"

#define MEMFREE  do { DB(F("mem=")); DBLN(freeMemory()); } while (0)

// Global variables ////////////////////////////////////////////////////////////////////////////

// Buffers of color data for rendering our effects 
CRGB Buffers[2][LedCount];  // Two buffers for two effects
CRGB LedData[LedCount];     // Mapped onto our LED strip

// Our two effects (for mixing together)
Effect* Effects[2] = { NULL, NULL };

// Used to keep track of which effect is next
uint8_t EffectIndex = 0;

typedef enum { None, JustA, JustB, AtoB, BtoA } t_BufferState;
t_BufferState BufferState = None;
    
// Used to control transitions
uint32_t LastTransitionStart = 0;

// Which way will be mix (0: EffectA@100%, EffectB@0%; 1: EffectA@0%, EffectB@100%)
uint8_t MixAmount = 128;

// Used to control frame rate
uint32_t LastLedUpdate = 0;

// For brightness control
uint8_t Brightness = 0;

// Functions ///////////////////////////////////////////////////////////////////////////////////

Effect* nextEffect(uint8_t buffer)
{
    Effect* effect = NULL;


        //effect = new FadeFlop(Buffers[buffer], PaletteGreen);
        //effect = new Blobs(Buffers[buffer], PaletteRed);

    if (EffectIndex==0) {
        effect = new Chase(Buffers[buffer], CloudColors_p);
    } else if (EffectIndex==1) {
        effect = new Chase(Buffers[buffer], LavaColors_p);
    } else if (EffectIndex==2) {
        effect = new Chase(Buffers[buffer], OceanColors_p);
    } else if (EffectIndex==3) {
        effect = new Chase(Buffers[buffer], ForestColors_p);
    } else if (EffectIndex==4) {
        effect = new Chase(Buffers[buffer], RainbowColors_p);
    } else if (EffectIndex==5) {
        effect = new Chase(Buffers[buffer], RainbowStripeColors_p);
    } else if (EffectIndex==6) {
        effect = new Chase(Buffers[buffer], PartyColors_p);
    } else if (EffectIndex==7) {
        effect = new Chase(Buffers[buffer], HeatColors_p);
    } else {
        DB(F("nextEffect: invalid EffectIndex "));
        DBLN(EffectIndex);
        effect = NULL;
    }

    EffectIndex = (EffectIndex+1) % 8;
    return effect;
}

void startTransition()
{
    switch(BufferState) {
    case JustA:
        DB(F("startTransition: starting AtoB "));
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
        DB(F("startTransition: starting BtoA "));
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
        // DBLN(F("startTransition: already transitioning"));
        break;
    }
}

void updateTransition()
{
    float complete = (float)(Millis() - LastTransitionStart) / TransitionMs;;
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

void ledClear(CRGB* dest, uint16_t count) 
{
    for(uint16_t i=0; i<count; i++) {
        dest[i] = CRGB::Black;
    }
}

void mixAdd(CRGB* src, CRGB* dest, uint16_t count, uint8_t scale=255) 
{
    for(uint16_t i=0; i<count; i++) {
        dest[i] = dest[i].lerp8(src[i], scale);
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
    Button.begin();
    BrightnessFader.begin(1,64,true);
    FastLED.addLeds<LedChipset, LedPin, LedOrder>(LedData, LedCount);

    BufferState = JustA;
    Effects[0] = nextEffect(0);
    MixAmount = 0;
    //EffectB = new Chase(Buffers[1], LedCount, BlueColorScheme, sizeof(BlueColorScheme)/sizeof((BlueColorScheme)[0]), 70, 1100);
    //EffectA = new FadeFlop(Buffers[0], LedCount, 2000, RedColorScheme, sizeof(RedColorScheme)/sizeof((RedColorScheme)[0]));
    //EffectB = new Chase(Buffers[0], LedCount, BlueColorScheme, sizeof(BlueColorScheme)/sizeof((BlueColorScheme)[0]), 70, 1100);

    MEMFREE;
    DBLN(F("E:setup"));
}

void loop()
{
    Button.update();
    BrightnessFader.update();
    updateTransition();

    if (Button.repeat(50,50)) {
        startTransition();
    }

    if (BrightnessFader.value() != Brightness) {
        DB(F("Brightness level="));
        DB(BrightnessFader.value());
        DB(F(" scale8="));
        DB((BrightnessFader.value()*4)-1);
        DB(' ');
        FastLED.setBrightness((BrightnessFader.value()*4)-1);
        Brightness = BrightnessFader.value();
        MEMFREE;
    }

    ledClear(LedData, LedCount);
    if (Effects[0]) { 
        Effects[0]->render(); 
        mixAdd(Buffers[0], LedData, LedCount, 255-MixAmount);
    }
    if (Effects[1]) { 
        Effects[1]->render(); 
        mixAdd(Buffers[1], LedData, LedCount, MixAmount);
    }
    ledUpdate();
}

