# Light Scultpure for Richard Raithby

## Hardware

* Single WS2812B LED strip - about 60 LEDs
* Push button to change light effect on strip
* Potential divider to adjust brightness
* Potential divider to adjust speed

## Software Features

* Effects:
    * Solid (turn entire strip a single color and don't change it - not used)
    * Sequence (set whole strip to colours from a palette)
    * Chase (scroll strip, changing first pixel colour periodically)
    * Blobs (slowly pulse on groups of pixels from colour within a palette)
    * Spurts (shoot spurts of colour from a palette along the strip with variable speed)
    * Sprakle (twinkle on pixels randomly using colours from a palette)
* Cross-fading between effects
* Button press changes to next effect
* Effect saved as default using EEPROM after 5 seconds of use
* Uses FastLED library

## TODO

* Reset before millis wrap
