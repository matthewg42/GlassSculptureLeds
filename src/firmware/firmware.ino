#include <math.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include <Heartbeat.h>
#include <DebouncedButton.h>
#include <DualButton.h>
#include <FastLED.h>
#include "Button.h"
#include "StripEffect.h"
#include "FlipFlop.h"
#include "Config.h"

Heartbeat HeartBeat(HeartbeatPin);
uint16_t pos = 0;
CRGB LedData[LedCount];
CRGB BufA[LedCount];
uint32_t LastLedUpdate = 0;
StripEffect* Effect = NULL;
CRGB ColorScheme[] = { CRGB::Red, CRGB::Yellow };

void ledClear(CRGB* dest, uint16_t count) {
    for(uint16_t i=0; i<count; i++) {
        dest[i] = CRGB::Black;
    }
}

void mixAdd(CRGB* src, CRGB* dest, uint16_t count) {
    for(uint16_t i=0; i<count; i++) {
        dest[i] += src[i];
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
    Effect = new FlipFlop(BufA, LedCount, 1000, ColorScheme, 2);
    DBLN(F("E:setup"));
}

void loop()
{
    Button.update();
    HeartBeat.update();

    ledClear(LedData, LedCount);
    if (Effect) { 
        Effect->render(); 
        mixAdd(BufA, LedData, LedCount);
    }
    ledUpdate();
}

