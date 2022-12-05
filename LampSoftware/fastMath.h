#include <Arduino.h>

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
#define FASTLED_RAND16_13849 ((uint16_t)(13849))
#define FASTLED_RAND16_2053  ((uint16_t)(2053))
#define APPLY_FASTLED_RAND16_2053(x) (x * FASTLED_RAND16_2053)
#define RAND16_SEED  1337

uint8_t c_sqrt16(uint16_t q);
uint8_t random8();
void setSeed(uint16_t seed);
