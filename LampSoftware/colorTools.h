#define LIB8STATIC __attribute__ ((unused)) static inline
#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline
//
// Partially from Fastled library
//
struct CRGBW {
	union {
		struct {
            union {
                uint8_t b;
                uint8_t blue;
            };
            union {
                uint8_t g;
                uint8_t green;
            };
            union {
                uint8_t r;
                uint8_t red;
            };
            union {
                uint8_t w;
                uint8_t white;
            };
        };
		uint8_t raw[4];
	};
    /// Array access operator to index into the crgb object
	inline uint8_t& operator[] (uint8_t x) __attribute__((always_inline))
    {
        return raw[x];
    }

    /// Array access operator to index into the crgb object
    inline const uint8_t& operator[] (uint8_t x) const __attribute__((always_inline))
    {
        return raw[x];
    }
    /// allow construction from R, G, B
    inline CRGBW( uint8_t ir, uint8_t ig, uint8_t ib, uint8_t iw)  __attribute__((always_inline))
        : b(ib), g(ig), r(ir), w(iw)
    {
    }	
 /// allow construction from 32-bit 0xRRGGBBWW color code
    inline CRGBW( uint32_t colorcode)  __attribute__((always_inline))
    : b((colorcode >> 0) & 0xFF), g((colorcode >> 8) & 0xFF),r((colorcode >> 16) & 0xFF),w((colorcode >> 24) & 0xFF)
    {
    }
};


uint8_t scaleColorValue(uint8_t & colorValue, uint8_t scale);
void cleanup_R1();
/// blend a variable proproportion(0-255) of one byte to another
/// @param a - the starting byte value
/// @param b - the byte value to blend toward
/// @param amountOfB - the proportion (0-255) of b to blend
/// @returns a byte value between a and b, inclusive
uint8_t blend8( uint8_t a, uint8_t b, uint8_t amountOfB);
CRGBW& nblend(CRGBW& existing, const CRGBW& overlay, uint8_t amountOfOverlay );
uint8_t scaleColor(uint32_t color, uint8_t scale);
uint32_t MergeColors(uint32_t color1,uint32_t color2,uint8_t percent);
uint8_t ease8InOutQuad(uint8_t val);