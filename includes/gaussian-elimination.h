#include <inttypes.h>
#include <stdlib.h>

typedef uint8_t Bit;

Bit *GF2GaussianElimination(size_t n, Bit A[n][n+1]);