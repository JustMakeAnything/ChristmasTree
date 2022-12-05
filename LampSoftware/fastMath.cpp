#include <Arduino.h>
#include "fastMath.h"

uint8_t c_sqrt16(uint16_t q){
    uint8_t r, mask;
    uint8_t i = 8*sizeof (r) -1;

    r = mask = 1 << i;
    
    for (; i > 0; i--)
    {
        mask >>= 1;
        
        if (q < (uint16_t) r*r)
            r -= mask;
        else
            r += mask;
    }
    
    if (q < (uint16_t) r*r)
        r -= 1;
    
    return r;
}

uint16_t random16seed = RAND16_SEED;


uint8_t random8(){
    random16seed = APPLY_FASTLED_RAND16_2053(random16seed) + FASTLED_RAND16_13849;
    // return the sum of the high and low bytes, for better
    //  mixing and non-sequential correlation
    return (uint8_t)(((uint8_t)(random16seed & 0xFF)) +
                     ((uint8_t)(random16seed >> 8)));
}

void setSeed(uint16_t seed){
    random16seed = seed;
}
