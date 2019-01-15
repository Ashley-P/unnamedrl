//#include <stdint.h>
//#include <stdlib.h>
#include "defs.h"

uint64_t xorshift64star(uint64_t *state) {
	uint64_t x = *state;	/* The state must be seeded with a nonzero value. */
	x ^= x >> 12; // a
	x ^= x << 25; // b
	x ^= x >> 27; // c
	*state = x;
	return x * 0x2545F4914F6CDD1D;
}

/* Returns a number between lower and upper inclusive */
int bounded_rng(int lower, int upper) {
    int k = (upper + 1) - lower;
    uint64_t t = (-k) % k;
    for (;;) {
        uint32_t r = xorshift64star(&globals.xorshift_state);
        if (r >= t)
            return (r % k) + lower;
    }
}
