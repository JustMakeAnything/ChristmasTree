///////////////////////////////////////////////////////////////////////////////
// Serial port Menus
// Adaption for my project
// Original:
// SerialMenu - Copyright (c) 2019 Dan Truong
///////////////////////////////////////////////////////////////////////////////
#define SERIALMENU_DISABLE_PROGMEM_SUPPORT false
#define SERIALMENU_MINIMAL_FOOTPRINT true
#include "CustomSerialMenu.h"

SerialMenu* SerialMenu::singleton = nullptr;
const SerialMenuEntry* SerialMenu::menu = nullptr;
uint8_t SerialMenu::size = uint8_t(0);
