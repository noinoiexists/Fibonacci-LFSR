#include <stdint.h>
#include <stdlib.h>

#define MAX_LENGTH 32

typedef uint8_t Bit;
typedef uint32_t LFSRState;

Bit *lfsr_compute(LFSRState state, LFSRState coeff, size_t m, size_t n);
