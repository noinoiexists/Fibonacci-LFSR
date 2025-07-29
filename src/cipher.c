#include <stdio.h>
#include <string.h>
#include "../includes/lfsr.h"

LFSRState binstr_to_bin(char *str);

int main() {
	char state_str[MAX_LENGTH];
	printf("Seed:\n");
	scanf("%s", state_str);
	char coeff_str[strlen(state_str)];
	printf("Feedback coefficients:\n");
	scanf("%s", coeff_str);
	if(strlen(coeff_str) != strlen(state_str)){
		printf("Lengths have to be equal\n");
		return 1;
	}
	getchar();
	

	LFSRState state = binstr_to_bin(state_str);
	LFSRState coeff = binstr_to_bin(coeff_str);

	printf("Plaintext:\n");
	char *input = NULL;
	size_t inputbuff_len = 0;
	ssize_t input_len = getline(&input, &inputbuff_len, stdin);
	if(input_len == -1 || input_len == 0) {
		fprintf(stderr, "Error getting input\n");
		return 1;
	}
	if (input[input_len-1] == '\n') {
    	input[--input_len] = '\0';
	}

	Bit *stream = lfsr_compute(state, coeff, strlen(coeff_str), (8*input_len));
	printf("Ciphertext (hex):\n");
	for(ssize_t i=0; i<input_len; ++i){
		unsigned char byte=0;
		for(size_t j=0; j<8; ++j){
			byte = (byte << 1) | stream[i*8 + j];
		}
		printf("%02x", (input[i]^byte));
	}
	printf("\n");

	free(stream); free(input);


	return 0;

}

LFSRState binstr_to_bin(char *str){
	size_t len = strlen(str);
	if (len >= MAX_LENGTH){
		fprintf(stderr, "Max length has been set to %d!\n", MAX_LENGTH);
		exit(1);
	}
	LFSRState bin = 0;
	for (size_t i=0; str[i] != '\0'; ++i) {
		if (str[len-1-i] == '1')
			bin |= (1 << i);
		else if (str[len-1-i] == '0')
			bin |= (0 << i);
		else {
			fprintf(stderr, "Input is not binary\n");
			exit(1);			
		}
	}
	return bin;
}


