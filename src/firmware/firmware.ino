#include <math.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include <DebouncedButton.h>
#include <DualButton.h>
#include <FastLED.h>
#include "Button.h"
#include "Config.h"

uint16_t pos = 0;
CRGB LedData[LedCount];
uint32_t LastLedUpdate = 0;

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
    FastLED.addLeds<LedChipset, LedPin, LedOrder>(LedData, LedCount);
    DBLN(F("Press button to paint color"));
}

void loop()
{
    Button.update();

    if (Button.repeat(10, 10)) {
        DB(Millis());
        DBLN(F(" Poink!"));
        if (pos < LedCount) {
            LedData[pos] = CRGB::Red;
        } else {
            LedData[pos%LedCount] = CRGB::Yellow;
        }
        pos = (pos + 1) % (LedCount * 2);
    }

    ledUpdate();
}

