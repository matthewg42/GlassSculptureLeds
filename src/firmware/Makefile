# Suppress a bunch of annoying warnings which come from the core Arduino code...
CPPFLAGS += -Wno-unused-variable -Wno-deprecated-declarations -Wno-cpp -Wno-write-strings

# Add flags for debugging and anything else we might have in our code...
# Build options:
# -DFIXEDBRIGHTNESS=255		override brightness control with fixed value
# -DDEBUG			enable debugging on serial interface
# -DHEARTBEAT			enable heartbeat
CPPFLAGS += -DDEBUG 

# Each library used should be added here. Use the directory name for the library as
# installed in your arduino libraries directory
ARDUINO_LIBS = Mutila FastLED EEPROM

# Select your board - uncomment as appropriate. I provided just a few
# common options here.  See the boards.txt file for more.  Some boards
# have just one option, like the uno, for these just provide a BOARD_TAG
# and find it from the first part of the line like this:
# uno.name=Arduino Uno
#BOARD_TAG = uno
#BOARD_TAG = leonardo
#
# Other boards have variants, e.g. the nano has two versions - the older
# one with the 168 or the more modern one with the 328 CPU.  The 168 is
# the default, so we select the alternative CPU like this:
BOARD_TAG = nano
BOARD_SUB = atmega328
ISP_PROG = usbasp
#
# Pro Mini 5V
#BOARD_TAG = pro
#BOARD_SUB = 16MHzatmega328

include ../arduino.mk

