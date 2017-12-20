#pragma once

#include <stdint.h>
#include <FastLED.h>
#include <Arduino.h>

// #defining CROSSFADE will use more memory (limit number of LEDs)
// but looks pretty swish...
#define             CROSSFADE               1

// Pin definitions
const uint8_t       LedPin =                A2;
const uint8_t       ButtonPin =             2;
const uint8_t       HeartbeatPin =          13;
const uint8_t       BrightnessFaderPin =    A4;
const bool          BrightnessFaderInvert = false;
const uint8_t       SpeedControlPin =       A5;
const bool          SpeedControlInvert =    false;

// LED parameters
#define             LedChipset              WS2812B
const uint16_t      LedCount =              100;
const EOrder        LedOrder =              GRB;
const uint16_t      LedRefreshMs =          10;

// Crossfade duration
const uint16_t      TransitionMs =          800;

// How long before we save a changed effect to EEPROM?
const uint16_t      EffectPersistenceMs =   5000;

// The mode button uses a pullup?
const bool          ButtonPullup =          true;


