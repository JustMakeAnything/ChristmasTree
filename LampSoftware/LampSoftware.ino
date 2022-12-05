#include <Arduino.h>
#include "constants.h"
#include "configuration.h"
#ifdef DEBUG
	#include <debug.h>
#endif
#ifdef MENU
	#include "menu.h"
#endif
#include "state.h"
#include "actions.h"

// When Building on ATMEGA 168 max 750 bytes of RAM should be
// used statically up to 30 LEDs

uint16_t touchdelay=9999;

#include "touch.h"
#include "patterns.h"

void setup() { 
    // Set the same bitrate in platformio.ini monitor_speed = xxx
    // or your serial console
	delay(50); // Serial Port delay
    Serial.begin(9600);
	delay(50); // Serial Port delay
    #ifdef DEBUG
	debugn('G',"Res");
	Serial.print("Res..");
	debugI('i');
	debugI(13.1f);
	showDebugOn();
	#endif
	
	setupPatterns();
	setupTouch();
	setupConfiguration();
	setupState();
	#ifdef MENU
		setupMenu();
	#endif
}

void loop() { 
	#ifdef MENU
		updateMenu();
	#endif
		// 1 cycle equals approximately to 50ms. 
		// Reading touch takes most of the time
		if(touchdelay >=TOUCH_CYCLES){
			readTouch(); // Only 2 Sensors are read each time
			touchdelay = 0;
		}else{
			touchdelay += 1;
		}
		executeActions();
		displayPattern();
		delay(20);
}