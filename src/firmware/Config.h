#pragma once

#include <stdint.h>
#include <FastLED.h>

#define             LedChipset              WS2812B

#ifdef LEDCOUNT
const uint16_t      LedCount =              LEDCOUNT;
#else
const uint16_t      LedCount =              100;
#endif
const EOrder        LedOrder =              GRB;
const uint8_t       LedPin =                4;
const uint16_t      LedRefreshMs =          10;

const uint16_t      TransitionMs =          800;

// Set to 0 for no saving / restoring of default effect
const uint16_t      EffectPersistenceMs =   5000;

// The mode button
const uint8_t       ButtonPin =             3;
const bool          ButtonPullup =          true;

const uint8_t       HeartbeatPin =          13;

const uint8_t       BrightnessFaderPin =    A7;
const uint8_t       SpeedControlPin =       A6;

