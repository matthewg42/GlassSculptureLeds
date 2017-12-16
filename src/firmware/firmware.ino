#include <math.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include <DebouncedButton.h>
#include <DiscretePot.h>
#include <FreeRam.h>
#include <FastLED.h>
#include "Button.h"
#include "BrightnessFader.h"
#include "SpeedControl.h"
#include "Effect.h"
#include "EffSequence.h"
#include "EffChase.h"
#include "EffBlobs.h"
#include "EffSolid.h"
#include "EffSparkle.h"
#include "EffSpurt.h"
#include "Palettes.h"
#include "Settings.h"
#include "Config.h"

// Macro to output free memory to serial in a nice readable way
#define MEMFREE  do { DB(F("mem=")); DBLN(FreeRam()); } while (0)

// Function prototypes added by protoize v2.00 at 2017-12-04 12:48:44
void setup();
void loop();
void ledClear(CRGB* dest, uint16_t count);
void ledUpdate();
void mixAdd(CRGB* src, CRGB* dest, uint16_t count, uint8_t scale=255);
Effect* nextEffect(uint8_t buffer, bool increment=true);
void startCrossfade();
void switchEffect();
void updateTransition();

// Global variables ////////////////////////////////////////////////////////////////////////////
#ifdef CROSSFADE
// Buffers of color data for rendering our effects 
CRGB Buffers[2][LedCount];              // Two buffers 
Effect* Effects[2] = { NULL, NULL };    // ...for two effects

typedef enum { None, JustA, JustB, AtoB, BtoA } t_BufferState;
t_BufferState BufferState = None;       // Keep track of transition state
uint32_t LastTransitionStart = 0;       // When transitions/switching starts
uint8_t MixAmount = 128;                // 0: EffectA@100%, EffectB@0%; 255: EffectA@0%, EffectB@100%
CRGB LedData[LedCount];                 // Mapped onto our LED strip by FastLED lib
uint8_t ButtonTaps = 0;                 // How many button presses do we have to react to?
#else
// no crossfade - we render directly into the LedData buffer, so don't need lots
// of those globals. Save the RAM!
CRGB Buffers[1][LedCount];              // This will be rendered directly
Effect* Effects[1] = { NULL };          // and attached to this effect
#endif

// Note: EffectIndex now a PersistentSetting, defined in Settings.cpp
uint32_t LastLedUpdate = 0;             // Used to control frame rate
uint32_t LastEffectSelected = 0;        // When the last effect came [fully] on
uint8_t Brightness = 0;                 // For brightness control by user

const uint8_t NumberOfEffects = 15;     // How many effects are we cycling through?

// Functions ///////////////////////////////////////////////////////////////////////////////////
void setup()
{
    Serial.begin(115200);

    // Seed RNG using noise in analog inputs from our setting pots and A0
    randomSeed((analogRead(A0)*1024*1024) + (analogRead(BrightnessFaderPin)*1024) + analogRead(SpeedControlPin));

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
    Effects[0] = nextEffect(0, false);
    
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
    // Give timeslice to various components
    Button.update();
    BrightnessFader.update();
    SpeedControl.update();

    // Adjust the speed control global if potential divider value has changed
    uint8_t speed8 = (SpeedControl.value()*4)-1;
    if (speed8 != SpeedFactor) {
        SpeedFactor = speed8;
        DB(F("Speed="));
        DBLN(SpeedFactor);
    }

    // Save the default effect if not already saved and has been selected for a while
    if (LastEffectSelected > 0 && EffectPersistenceMs > 0 && MillisSince(LastEffectSelected) > EffectPersistenceMs) {
        DBLN(F("Saving default effect"));
        LastEffectSelected = 0;
        EffectIndex.save();
    }

    // Handle the effect change push button
#ifdef CROSSFADE
    if (Button.tapped() && ButtonTaps < 255) {
        ButtonTaps++;
        DB(F("ButtonTaps="));
        DBLN(ButtonTaps);
    }
    if (ButtonTaps > 0 && (BufferState == JustA || BufferState == JustB)) {
        ButtonTaps--;
        startCrossfade();
    }
#else
    if (Button.tapped()) {
        switchEffect();
    }
#endif

#ifdef CROSSFADE
    updateTransition();
#endif

    // Adjust the brightness if the brightness potential divider position has changed
    if (BrightnessFader.value() != Brightness) {
        DB(F("Brightness level="));
        DB(BrightnessFader.value());
        DB(F(" scale8="));
        DBLN((BrightnessFader.value()*4)-1);
#ifdef FIXEDBRIGHTNESS
        FastLED.setBrightness(FIXEDBRIGHTNESS);
#else
        FastLED.setBrightness((BrightnessFader.value()*4)-1);
#endif
        Brightness = BrightnessFader.value();
    }

    // Render to the LED strip buffer
#ifdef CROSSFADE
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

    // Send LED buffer to LEDs
    ledUpdate();
}

Effect* nextEffect(uint8_t buffer, bool increment)
{
    Effect* effect = NULL;

    if (increment) {
        EffectIndex = EffectIndex.get() + 1;
    }

    // wrap and/or deal with bad value from EEPROM
    EffectIndex = EffectIndex.get() % NumberOfEffects;

    DB(F("EffectIndex="));
    DBLN(EffectIndex.get());

    if (EffectIndex.get()==0) {
        effect = new EffSequence(Buffers[buffer], RedOrangeColors_p);
    } else if (EffectIndex.get()==1) {
        effect = new EffSequence(Buffers[buffer], BlueGreenColors_p);
    } else if (EffectIndex.get()==2) {
        effect = new EffSequence(Buffers[buffer], RainbowColors_p);
    } else if (EffectIndex.get()==3) {
        effect = new EffBlobs(Buffers[buffer], RedOrangeColors_p);
    } else if (EffectIndex.get()==4) {
        effect = new EffBlobs(Buffers[buffer], BlueGreenColors_p);
    } else if (EffectIndex.get()==5) {
        effect = new EffBlobs(Buffers[buffer], RainbowColors_p);
    } else if (EffectIndex.get()==6) {
        effect = new EffChase(Buffers[buffer], RedOrangeColors_p);
    } else if (EffectIndex.get()==7) {
        effect = new EffChase(Buffers[buffer], BlueGreenColors_p);
    } else if (EffectIndex.get()==8) {
        effect = new EffChase(Buffers[buffer], RainbowColors_p);
    } else if (EffectIndex.get()==9) {
        effect = new EffSpurt(Buffers[buffer], RedOrangeColors_p);
    } else if (EffectIndex.get()==10) {
        effect = new EffSpurt(Buffers[buffer], BlueGreenColors_p);
    } else if (EffectIndex.get()==11) {
        effect = new EffSpurt(Buffers[buffer], RainbowColors_p);
    } else if (EffectIndex.get()==12) {
        effect = new EffSparkle(Buffers[buffer], RedOrangeColors_p);
    } else if (EffectIndex.get()==13) {
        effect = new EffSparkle(Buffers[buffer], BlueGreenColors_p);
    } else if (EffectIndex.get()==14) {
        effect = new EffSparkle(Buffers[buffer], RainbowColors_p);
    } else {
        DB(F("ERROR: invalid EffectIndex="));
        DBLN(EffectIndex.get());
        effect = NULL;
    }

    DB(F("Memory during transition: "));
    MEMFREE;
    return effect;
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
    float complete = (float)(MillisSince(LastTransitionStart)) / TransitionMs;
    switch(BufferState) {
    case AtoB:
        if (complete >= 1.0) {
            LastEffectSelected = Millis();
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
        complete = (float)(MillisSince(LastTransitionStart)) / TransitionMs;
        if (complete >= 1.0) {
            LastEffectSelected = Millis();
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

void mixAdd(CRGB* src, CRGB* dest, uint16_t count, uint8_t scale) 
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
    LastEffectSelected = Millis();
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
    if (LastLedUpdate == 0 || MillisSince(LastLedUpdate) > LedRefreshMs) {
        FastLED.show();
        AddMillisOffset(LedCount*0.0215);
        LastLedUpdate = Millis();
    }
}

