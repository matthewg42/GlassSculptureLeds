#pragma once

#include <stdint.h>
#include <FastLED.h>

#ifndef NOCROSSFADE
#define             CROSSFADE               1
#endif

// Pin definitions
const uint8_t       LedPin =                4;
const uint8_t       ButtonPin =             3;
const uint8_t       HeartbeatPin =          13;
const uint8_t       BrightnessFaderPin =    A7;
const uint8_t       SpeedControlPin =       A6;

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


