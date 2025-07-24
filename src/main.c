#include <stdio.h>
#include <string.h>
#include "lfsr.h"

LFSRState str_to_bin(char *str);

int main() {
	size_t n;
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
	printf("Length of bit stream required:\n");
	scanf("%ld", &n);

	LFSRState state = str_to_bin(state_str);
	LFSRState coeff = str_to_bin(coeff_str);

	Bit *stream = lfsr_compute(state, coeff, strlen(coeff_str), n);
	for(size_t i=0; i<n; ++i){
		printf("%d", stream[i]);
	}
	free(stream);
	printf("\n");

	return 0;

}

LFSRState str_to_bin(char *str){
	size_t len = strlen(str);
	if (len >= MAX_LENGTH){
		printf("Max length has been set to %d!\n", MAX_LENGTH);
		exit(1);
	}
	LFSRState bin = 0;
	for (size_t i=0; str[i] != '\0'; ++i) {
		if (str[len-1-i] == '1')
			bin |= (1 << i);
		else if (str[len-1-i] == '0')
			bin |= (0 << i);
		else {
			printf("Input is not binary");
			exit(1);			
		}
	}
	return bin;
}

