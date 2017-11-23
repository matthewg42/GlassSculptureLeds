#pragma once

#include <stdint.h>
#include <FastLED.h>

#define             LedChipset              WS2812B
const uint16_t      LedCount =              150;
const EOrder        LedOrder =              GRB;
const uint8_t       LedPin =                4;
const uint16_t      LedRefreshMs =          10;

const uint8_t       WiredButtonPin =        3;
const uint8_t       RemoteButtonPin =       5;
const bool          WiredButtonPullup =     true;
const bool          RemoteButtonPullup =    true;

const uint8_t       HeartbeatPin =          13;
