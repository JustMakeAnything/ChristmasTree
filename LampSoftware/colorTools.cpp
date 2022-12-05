#include <Arduino.h>
#include "colorTools.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

uint8_t scaleColorValue(uint8_t & colorValue, uint8_t scale){
    colorValue = (((int)colorValue * (int)scale) >> 8) + ((colorValue&&scale)?1:0);
    return colorValue;
/*
    asm volatile(
        "  tst %[i]\n\t"
        "  breq L_%=\n\t"
        "  mul %[i], %[scale]\n\t"
        "  mov %[i], r1\n\t"
        "  breq L_%=\n\t"
        "  subi %[i], 0xFF\n\t"
        "L_%=: \n\t"
        : [i] "+a" (i)
        : [scale] "a" (scale)
        : "r0", "r1"
    );	
	*/ 
}
 void cleanup_R1(){
    // Restore r1 to "0"; it's expected to always be that
    asm volatile( "clr __zero_reg__  \n\t" : : : "r1" );
}

// From Fastled library
/// blend a variable proproportion(0-255) of one byte to another
/// @param a - the starting byte value
/// @param b - the byte value to blend toward
/// @param amountOfB - the proportion (0-255) of b to blend
/// @returns a byte value between a and b, inclusive
 uint8_t blend8( uint8_t a, uint8_t b, uint8_t amountOfB)
{

    // The BLEND_FIXED formula is
    //
    //   result = (  A*(amountOfA) + B*(amountOfB)              )/ 256
    //
    // …where amountOfA = 255-amountOfB.
    //
    // This formula will never return 255, which is why the BLEND_FIXED + SCALE8_FIXED version is
    //
    //   result = (  A*(amountOfA) + A + B*(amountOfB) + B      ) / 256
    //
    // We can rearrange this formula for some great optimisations.
    //
    //   result = (  A*(amountOfA) + A + B*(amountOfB) + B      ) / 256
    //          = (  A*(255-amountOfB) + A + B*(amountOfB) + B  ) / 256
    //          = (  A*(256-amountOfB) + B*(amountOfB) + B      ) / 256
    //          = (  A*256 + B + B*(amountOfB) - A*(amountOfB)  ) / 256  // this is the version used in SCALE8_FIXED AVR below
    //          = (  A*256 + B + (B-A)*(amountOfB)              ) / 256  // this is the version used in SCALE8_FIXED C below

    uint16_t partial;
    uint8_t result;
    // 1 or 2 cycles depending on how the compiler optimises
    partial = (a << 8) | b;

    // 7 cycles
    asm volatile (
        "  mul %[a], %[amountOfB]        \n\t"
        "  sub %A[partial], r0           \n\t"
        "  sbc %B[partial], r1           \n\t"
        "  mul %[b], %[amountOfB]        \n\t"
        "  add %A[partial], r0           \n\t"
        "  adc %B[partial], r1           \n\t"
        "  clr __zero_reg__              \n\t"
        : [partial] "+r" (partial)
        : [amountOfB] "r" (amountOfB),
          [a] "r" (a),
          [b] "r" (b)
        : "r0", "r1"
    );
    result = partial >> 8;
    return result;
}
CRGBW& nblend(CRGBW& existing, const CRGBW& overlay, uint8_t amountOfExisting )
{
    if( amountOfExisting == 255) {
        return existing;
    }
    if( amountOfExisting == 0) {
        existing = overlay;
        return existing;
    }
    // Corrected blend method, with no loss-of-precision rounding errors
    existing.red   = blend8( existing.red,   overlay.red,   255-amountOfExisting);
    existing.green = blend8( existing.green, overlay.green, 255-amountOfExisting);
    existing.blue  = blend8( existing.blue,  overlay.blue,  255-amountOfExisting);
    existing.white = blend8( existing.white, overlay.white, 255-amountOfExisting);
    return existing;
}
uint8_t scaleColor(uint32_t color, uint8_t scale){
//    return (((int) * (int)scale) >> 8) + ((i&&scale)?1:0);
return 0;
}

uint32_t MergeColors(uint32_t color1,uint32_t color2,uint8_t percent){
	#ifdef FASTLED
	#endif
					return 0;	
	#ifdef FASTLED
		}	
	#endif
}

uint8_t ease8InOutQuad(uint8_t val) {
    uint8_t j=val;
    asm volatile (
      "sbrc %[val], 7 \n"
      "com %[j]       \n"
      "mul %[j], %[j] \n"
      "add r0, %[j]   \n"
      "ldi %[j], 0    \n"
      "adc %[j], r1   \n"
      "lsl r0         \n" // carry = high bit of low byte of mul product
      "rol %[j]       \n" // j = (j * 2) + carry // preserve add'l bit of precision
      "sbrc %[val], 7 \n"
      "com %[j]       \n"
      "clr __zero_reg__   \n"
      : [j] "+&a" (j)
      : [val] "a" (val)
      : "r0", "r1"
      );
    return j;
}