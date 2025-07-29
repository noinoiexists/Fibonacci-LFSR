#include <stdio.h>
#include <string.h>
#include "../includes/gaussian-elimination.h"
#include "../includes/lfsr.h"

#define MIN_SEED_LEN 2

int main() {

	printf("Ciphertext (hex):\n");
	char *ciphertext = NULL;
	size_t ciphertext_buff_len=0;
	ssize_t ciphertext_len = getline(&ciphertext, &ciphertext_buff_len, stdin);
	if(ciphertext_len == -1 || ciphertext_len == 0) {
		fprintf(stderr, "Error getting input\n");
		return 1;
	}
	if (ciphertext[ciphertext_len-1] == '\n') {
    	ciphertext[--ciphertext_len] = '\0';
	}

	printf("Known Plaintext:\n");
	char *plaintext = NULL;
	size_t plaintext_buff_len=0;
	ssize_t plaintext_len = getline(&plaintext, &plaintext_buff_len, stdin);
	if(plaintext_len == -1 || plaintext_len == 0) {
		fprintf(stderr, "Error getting input\n");
		return 1;
	}
	if (plaintext[plaintext_len-1] == '\n') {
    	plaintext[--plaintext_len] = '\0';
	}

	unsigned char ciphertext_bytes[ciphertext_len/2];
	for (size_t i = 0; i < (size_t)(ciphertext_len/2); ++i) {
		char ciphertext_hex_byte[3] = {ciphertext[2 * i], ciphertext[2 * i + 1], '\0'};
		ciphertext_bytes[i] = (unsigned char)strtol(ciphertext_hex_byte, NULL, 16);
	}
	free(ciphertext);

	Bit flag=0;
	for (size_t poss_m = MIN_SEED_LEN; poss_m<=8*(size_t)plaintext_len; ++poss_m) {
		if (8 * (size_t)plaintext_len < 2 * poss_m || (size_t)(ciphertext_len/2) < (2 * poss_m + 7) / 8) {
			continue;
		}
		Bit *s = malloc(2 * poss_m * sizeof(Bit));
		if (!s) {
			fprintf(stderr, "Memory allocation failed\n");
			return 1;
		}
		for(size_t i=0; i< 2*poss_m; ++i)
			s[i] = ((plaintext[i/8] >> (7-(i%8))) & 1)^((ciphertext_bytes[i/8] >> (7-(i%8))) & 1);

		Bit A[poss_m][poss_m+1];
		for (size_t i=0; i<poss_m; ++i){
			for(size_t j=0; j<poss_m; ++j){
				A[i][j] = s[i+j];
			}
			A[i][poss_m] = s[i+poss_m];
		}
		Bit *p = GF2GaussianElimination(poss_m, A);
		if (!p) {
			free(s);
			continue;
		}
		
		

		flag = 1;
		LFSRState coeff = 0;
		for (size_t i = 0; i < poss_m; ++i)
			coeff |= ((LFSRState)p[i] << i); 

		LFSRState seed = 0;
		for (size_t i = 0; i < poss_m; ++i) 
			seed |= ((LFSRState)s[i] << i);  
		

		Bit *stream = lfsr_compute(seed, coeff, poss_m, (ciphertext_len*4));
		if (!stream) {
			fprintf(stderr, "LFSR computation failed\n");
			free(s); free(p);
			continue;
		}
		
		unsigned char *poss_plaintext = malloc((ciphertext_len/2)*sizeof(unsigned char)); 
		for(size_t i=0; i<(size_t)(ciphertext_len/2); ++i){
			unsigned char byte=0;
			for(size_t j=0; j<8; ++j){
				byte = (byte << 1) | stream[i*8 + j];
			}
			poss_plaintext[i] = (ciphertext_bytes[i]^byte);
		}
		for(size_t i=0; i<(size_t)plaintext_len; ++i){
			if(poss_plaintext[i] != plaintext[i]){
				flag = 0;
				free(poss_plaintext);
				break;
			}
		}
		 
		free(stream);

		if(flag){
			printf("Found Seed length:\n%lu\n", poss_m);
			printf("Found feedback coefficients:\n");
			for(size_t i=0; i<poss_m; ++i)
				printf("%u", p[poss_m-1-i]);
			printf("\nFound plaintext:\n");
			for(size_t i=0; i<(size_t)(ciphertext_len/2); ++i)
				printf("%c", poss_plaintext[i]);
			printf("\n");
			free(p); free(poss_plaintext);

			break;
		}
		free(p); free(s);			

	}
	if(!flag)
		printf("Insufficient data\n");
	
	free(plaintext);
}