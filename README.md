# Light Scultpure for Richard Raithby

## Hardware

* Single WS2812B LED strip - 100 LEDs, physically split into chunks, but one contiguous strip from a s/w point of view
* Push button to change light effect on strip
* Potential divider to adjust brightness
* Potential divider to adjust speed

## Software Features

* Effects:
    * Sequence (set whole strip to colours from a palette)
    * Chase (scroll strip, changing first pixel colour periodically)
    * Blobs (slowly pulse on groups of pixels from colour within a palette)
    * Spurts (shoot spurts of colour from a palette along the strip with variable speed)
    * Sparkle (twinkle on pixels randomly using colours from a palette)
* Cross-fading between effects
* Button press changes to next effect
* Effect saved as default using EEPROM after 5 seconds of use
* Uses FastLED library

## Dependencies

* Arduino IDE v1.8.5 (although other versions will probably work)
* FastLED v3.1.6 (install through Arduino library manager)
* [Mutila](https://github.com/matthewg42/Mutila) dev branch 2d4e60a or later, (or v1.1.2 when released)

## Future work

* Spurts in both directions
* Reverse direction option for Chase
* Blobs chunk size to fit in LED segments
* Wear levelling for effect persistence

# Delivery

Production releases will be tagged, with library versions in tag comments.

