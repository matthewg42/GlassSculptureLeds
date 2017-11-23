#include <math.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include <Heartbeat.h>
#include <DebouncedButton.h>
#include <FastLED.h>
#include "Button.h"
#include "StripEffect.h"
#include "FlipFlop.h"
#include "FadeFlop.h"
#include "Chase.h"
#include "FadeChase.h"
#include "Blobs.h"
#include "Config.h"

Heartbeat HeartBeat(HeartbeatPin);
uint16_t pos = 0;
CRGB LedData[LedCount];
CRGB BufA[LedCount];
CRGB BufB[LedCount];
uint32_t LastLedUpdate = 0;
StripEffect* EffectA = NULL;
StripEffect* EffectB = NULL;
CRGB RedColorScheme[] = { CRGB::Red, CRGB::LightYellow, CRGB::PaleVioletRed, CRGB::DarkRed, 
                          CRGB::Yellow, CRGB::MediumVioletRed, CRGB::Orange, CRGB::White };
CRGB BlueColorScheme[] = { CRGB::DeepSkyBlue, CRGB::Cyan, CRGB::DarkBlue, CRGB::CornflowerBlue, 
                           CRGB::BlueViolet, CRGB::SteelBlue, CRGB::MidnightBlue, CRGB::LightSteelBlue };
uint8_t mixAmount = 255;

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
    HeartBeat.begin();
    FastLED.addLeds<LedChipset, LedPin, LedOrder>(LedData, LedCount);
    EffectA = new Blobs(BufA, LedCount, RedColorScheme, sizeof(RedColorScheme)/sizeof((RedColorScheme)[0]));
    //EffectA = new FadeFlop(BufA, LedCount, 2000, RedColorScheme, sizeof(RedColorScheme)/sizeof((RedColorScheme)[0]));
    // EffectB = new FadeChase(BufB, LedCount, BlueColorScheme, sizeof(BlueColorScheme)/sizeof((BlueColorScheme)[0]), 70, 1100);
    // EffectB = new Chase(BufB, LedCount, BlueColorScheme, sizeof(BlueColorScheme)/sizeof((BlueColorScheme)[0]), 70, 1100);
    DBLN(F("E:setup"));
}

void loop()
{
    Button.update();
    HeartBeat.update();

    if (Button.tapped()) {
        DBLN(F("Button press: starting transition"));
    }

    ledClear(LedData, LedCount);
    if (EffectA) { 
        EffectA->render(); 
        mixAdd(BufA, LedData, LedCount, mixAmount);
    }
    if (EffectB) { 
        EffectB->render(); 
        mixAdd(BufB, LedData, LedCount, 255-mixAmount);
    }
    ledUpdate();
}

