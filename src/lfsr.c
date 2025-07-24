#include <stdint.h>
#include <stdlib.h>

typedef uint8_t Bit;
typedef uint32_t LFSRState;

void lfsr_shift(LFSRState *state, Bit feedback_bit, size_t m){
	*state = *state >> 1;
	*state |= ((LFSRState)feedback_bit << (m-1));
}

Bit *lfsr_compute(LFSRState state, LFSRState coeff, size_t m, size_t n) {
	Bit *stream = malloc(n * sizeof(Bit));
	if (!stream) return NULL;

	for(size_t i=0; i<n; ++i){
		Bit feedback_bit = 0;
		for(size_t j=0; j<m;++j){
			if ((coeff >> j) & 1)
				feedback_bit ^= (state >> j) & 1;
		}
		stream[i] = state & 1;
		lfsr_shift(&state, feedback_bit, m);
	}

	return stream;
}
