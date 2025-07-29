#include <stdio.h>
#include <limits.h>
#include "../includes/gaussian-elimination.h"

void swap_rows(size_t n, Bit A[n][n+1], size_t row1, size_t row2);

Bit *GF2GaussianElimination(size_t n, Bit A[n][n+1]){
	size_t min_row;
	for (size_t i=0; i<n; ++i){
		// Pivoting
		min_row=i;
		for (size_t j = i; j < n; ++j) {
			if (A[j][i] == 1) {
				min_row = j;
				break;
			}
		}

		
		if (i != min_row)
			swap_rows(n, A, i, min_row);
		// Forward Elimination
		for (size_t j=1; i+j<n; ++j){
			if (A[i+j][i]) {
				for (size_t k=0; k<=n; ++k){
					A[i+j][k] ^= A[i][k];
				}
			}
		}

	}
	// Back Substitution
	Bit *soln = malloc(sizeof(Bit)*n);
	if (!soln){
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}
	for (int32_t i=n-1; i>=0; --i){
		Bit C = A[i][n];
		for (size_t j=1; i+j<n; ++j){
			C ^= A[i][i+j] & soln[i+j];
		}
		soln[i] = C;
	}
	
	return soln;
}

void swap_rows(size_t n, Bit A[n][n+1], size_t row1, size_t row2) {
	for(size_t i=0; i<=n; ++i){
		Bit temp = A[row1][i];
		A[row1][i] = A[row2][i];
		A[row2][i] = temp;
	}
}