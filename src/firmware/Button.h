#pragma once

#include <DebouncedButton.h>
#include <DualButton.h>

// The global instance of DualButton for our control button
extern DualButton Button;

// The physical buttons (pin inputs) which make it up...
extern DebouncedButton WiredButton;
extern DebouncedButton RemoteButton;


