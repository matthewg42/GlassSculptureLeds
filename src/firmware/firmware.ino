#include <math.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <FastLED.h>
#include "Config.h"

uint16_t pos = 0;
CRGB LedData[LedCount];
DebouncedButton Button(ButtonPin);

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
        DB(millis());
        DBLN(F(" Poink!"));
        if (pos < LedCount) {
            LedData[pos] = CRGB::Red;
        } else {
            LedData[pos%LedCount] = CRGB::Yellow;
        }
        pos = (pos + 1) % (LedCount * 2);
    }

    FastLED.show();
}

