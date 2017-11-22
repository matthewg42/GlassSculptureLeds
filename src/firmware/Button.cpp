#include "Button.h"
#include "Config.h"

DebouncedButton WiredButton(WiredButtonPin, WiredButtonPullup);
DebouncedButton RemoteButton(RemoteButtonPin, RemoteButtonPullup);
DualButton Button(&WiredButton, &RemoteButton);

