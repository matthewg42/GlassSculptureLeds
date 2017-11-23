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
#include "StripEffect.h"
#include "FlipFlop.h"
#include "FadeFlop.h"
#include "Chase.h"
#include "FadeChase.h"
#include "Blobs.h"
#include "Config.h"

#define MEMFREE  do { DB(F("mem=")); DBLN(freeMemory()); } while (0)

// Global variables ////////////////////////////////////////////////////////////////////////////
// Buffers of color data for rendering our effects 
CRGB Buffers[2][LedCount];  // Two buffers for two effects
CRGB LedData[LedCount];     // Mapped onto our LED strip

// Our two effects (for mixing together)
StripEffect* EffectA = NULL;
StripEffect* EffectB = NULL;

// Which way will be mix (0: EffectA@100%, EffectB@0%; 1: EffectA@0%, EffectB@100%)
uint8_t mixAmount = 128;

// Used to control frame rate
uint32_t LastLedUpdate = 0;

// Color schemes for our effects
const CRGB RedColorScheme[]     = { CRGB::Red, CRGB::OrangeRed, CRGB::Crimson };
const CRGB BlueColorScheme[]    = { CRGB::DeepSkyBlue, CRGB::CornflowerBlue, CRGB::MidnightBlue };
const CRGB GreenColorScheme[]   = { CRGB::Green, CRGB::LawnGreen, CRGB::DarkOliveGreen };
const CRGB YellowColorScheme[]  = { CRGB::Yellow, CRGB::LightGoldenrodYellow, CRGB::GreenYellow };
const CRGB PurpleColorScheme[]  = { CRGB::Purple, CRGB::MediumOrchid, CRGB::MediumPurple };

// For brightness control
uint8_t Brightness = 0;

// Functions ///////////////////////////////////////////////////////////////////////////////////

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
    EffectA = new Blobs(Buffers[0], LedCount, RedColorScheme, sizeof(RedColorScheme)/sizeof((RedColorScheme)[0]));
    EffectB = new Chase(Buffers[1], LedCount, BlueColorScheme, sizeof(BlueColorScheme)/sizeof((BlueColorScheme)[0]), 70, 1100);
    //EffectA = new FadeFlop(Buffers[0], LedCount, 2000, RedColorScheme, sizeof(RedColorScheme)/sizeof((RedColorScheme)[0]));
    //EffectB = new Chase(Buffers[0], LedCount, BlueColorScheme, sizeof(BlueColorScheme)/sizeof((BlueColorScheme)[0]), 70, 1100);

    MEMFREE;
    DBLN(F("E:setup"));
}

void loop()
{
    Button.update();
    BrightnessFader.update();

    if (Button.tapped()) {
        DBLN(F("Button press: starting transition"));
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
    if (EffectA) { 
        EffectA->render(); 
        mixAdd(Buffers[0], LedData, LedCount, mixAmount);
    }
    if (EffectB) { 
        EffectB->render(); 
        mixAdd(Buffers[1], LedData, LedCount, 255-mixAmount);
    }
    ledUpdate();
}

