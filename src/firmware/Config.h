#pragma once

#include <stdint.h>
#include <FastLED.h>

#define             LedChipset              WS2812B
//const uint16_t      LedCount =              150;
const uint16_t      LedCount =              48;
const EOrder        LedOrder =              GRB;
const uint8_t       LedPin =                4;
const uint16_t      LedRefreshMs =          10;

const uint16_t      TransitionMs =          2000;

// The mode button
const uint8_t       ButtonPin =             3;
const bool          ButtonPullup =          true;

const uint8_t       HeartbeatPin =          13;

const uint8_t       BrightnessFaderPin =    A7;
const uint8_t       SpeedControlPin =       A6;

