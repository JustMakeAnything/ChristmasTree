#include <Adafruit_NeoPixel.h>

// The type of strip can be defined here (with: NEO+GRBW or without white: NEO+GRB)
// Sequence of colors can be changed: GRB is common, but other like BGR are also possible 
// R=Red G=Green B=Blue
#define STRIP_TYPE NEO_GRBW
// These numbers influence the static memory consumption
#define MAX_SENSORS 4  // Up to 6 possible
#define MAXPATTERNS 6


// Pattern max values
#define MAXPOSITIONS 8  // Patterns can use this much elements
#define MAX_NUM_LEDS 25 // only byte values are possible max(255) Memory restriction


// Conditional compiling to save memory
#define MENU
#define MENU_LED_STRIP
#define MENU_PATTERNS
#define MENU_FUNCTIONS
//#define MENU_LED_ORDER   // Work in progress...

#define DEFAULT_NUM_LEDS 15


// Save memory, but texts may be more cryptic
#define LOWMEM x

// How many leds in your strip(s)?
// For Atmega 168 MAX_LED_STRIP_COUNT * MAX_NUM_LEDS cannot exceed 30,
// otherwise the software will crash
#define MAX_LED_STRIP_COUNT 1

// Data pins of both currently possible strips
#define DATA_PIN1 2
#define DATA_PIN2 3

// Touch reaction time
#define TOUCH_CYCLES 1

// The following settings are only used for battery powered
// devices and alternative touch sensing
// Due to memory issues not possible on 168

// Pin where all 2 MOhm resistors are connected
#define TOUCH_COMMON 5
// Pins where the touchpads are connected
#define TOUCH_ONOFF 8
#define TOUCH_2 9
#define TOUCH_3 7



// Uncomment DEBUG to get information on the serial console
// Probably not possible for Atemga 168 when other options are active
// due to memory issues
//#define DEBUG


// Change this value to any other value to reset the configuration
#define MAGICCODE 113
