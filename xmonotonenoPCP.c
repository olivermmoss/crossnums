#include <stdio.h>
#include <stdlib.h>
#include "bankers.c"

// number of vertices
int N = 13;
unsigned long Nchoose5 = (N * (N-1) * (N-2) * (N-3) * (N-4)) / 120;
unsigned long Nchoose4 = (N * (N-1) * (N-2) * (N-3)) / 24;
unsigned long Nchoose3 = (N * (N-1) * (N-2)) / 6;

// searching for at most/exactly this many pseudoconvex pentagons
// int k = 31;
// bool exact = false;

int minBankers3;
int minBankers5;

// p(i,j) is whether the edge btwn i and j is inside or outside.
// note that we only use this when i < j - 1, otherwise it's a useless var
// but encoding it this way is way easier lol - we just set useless vars to true
int s(int i, int j, int k) {
	// plus one is to move it from [0, N) to [1, N]
	unsigned long pos = ((1 << i) + (1 << j) + (1 << k));
	
	return 1 + inverse(pos) - minBankers3;
}

// q(a,b,c,d,e) is whether those vtxs are a pseudo-convex pentagon
/*int q(int a, int b, int c, int d, int e) {
	// don't think I need to do any casting as long as N < 31
	unsigned long pos = ((1 << a) + (1 << b) + (1 << c) + (1 << d) + (1 << e));
	
	return 1 + Nchoose2 + inverse(pos) - minBankers5;
}*/

int main() {
	length = N;
	minBankers5 = inverse((1 << N-1) + (1 << N-2) + (1 << N-3) + (1 << N-4) + (1 << N-5));
	minBankers3 = inverse((1 << N-1) + (1 << N-2) + (1 << N-3));

  FILE *file_pointer;
  const char *filename = "xmononoPCP.cnf"; // Specify the name of the file

  // Open the file in write mode ("w"). If the file doesn't exist, it will be created.
  // If the file exists, it will be overwritten.
  file_pointer = fopen(filename, "w");

  if (file_pointer == NULL) {
    printf("Error opening the file.\n");
    return 1; // Indicate an error
  }

  // Write data to the file
	unsigned long Nclauses = (6 * Nchoose4 + 4 * Nchoose5);

  fprintf(file_pointer, "p cnf %lu %lu\n", Nchoose3, Nclauses);
	// it's + 2*(Nchoose2 - N + 1) if we're doing symmetry  

	// forbid the BAD GROUPS of 4
	for(int a = 0; a < N - 3; a++) {
		for(int b = a+1; b < N - 2; b++) {
			for(int c = b+1; c < N - 1; c++) {
				for(int d = c+1; d < N; d++) {
						// preserve semisimplicity
						fprintf(file_pointer, "%d %d %d %d 0\n", -s(a,b,c), s(a,b,d), -s(a,c,d), s(b,c,d));
						fprintf(file_pointer, "%d %d %d %d 0\n", -s(a,b,c), s(a,b,d), -s(a,c,d), -s(b,c,d));
						fprintf(file_pointer, "%d %d %d %d 0\n", -s(a,b,c), -s(a,b,d), s(a,c,d), -s(b,c,d));
						fprintf(file_pointer, "%d %d %d %d 0\n", s(a,b,c), -s(a,b,d), s(a,c,d), -s(b,c,d));
						fprintf(file_pointer, "%d %d %d %d 0\n", s(a,b,c), -s(a,b,d), s(a,c,d), s(b,c,d));
						fprintf(file_pointer, "%d %d %d %d 0\n", s(a,b,c), s(a,b,d), -s(a,c,d), s(b,c,d));
	}}}}

	// forbid the BAD GROUPS of 5
	for(int a = 0; a < N - 4; a++) {
		for(int b = a+1; b < N - 3; b++) {
			for(int c = b+1; c < N - 2; c++) {
				for(int d = c+1; d < N - 1; d++) {
					for(int e = d+1; e < N; e++) {
						// preserve simplicity
						fprintf(file_pointer, "%d %d %d %d 0\n", s(a,b,e), s(a,d,e), s(b,c,d), -s(a,c,e));
						fprintf(file_pointer, "%d %d %d %d 0\n", -s(a,b,e), -s(a,d,e), -s(b,c,d), s(a,c,e));
						// no PCPs:
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", s(a,b,c), s(a,b,d), s(a,c,d), s(b,c,d), s(b,c,e), s(b,d,e), s(c,d,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -s(a,b,c), -s(a,b,d), -s(a,c,d), -s(b,c,d), -s(b,c,e), -s(b,d,e), -s(c,d,e));
	}}}}}

	// finally, we add the knf clauses:

	/*if(exact) {
		fprintf(file_pointer, "k %d ", k);
		for(int a = 0; a < N - 4; a++) {
			for(int b = a+1; b < N - 3; b++) {
				for(int c = b+1; c < N - 2; c++) {
					for(int d = c+1; d < N - 1; d++) {
						for(int e = d+1; e < N; e++) {
							fprintf(file_pointer, "%d ", q(a,b,c,d,e));
		}}}}}
		fprintf(file_pointer, "0\n");
	}
	fprintf(file_pointer, "k %lu ", Nchoose5 - k);
	for(int a = 0; a < N - 4; a++) {
		for(int b = a+1; b < N - 3; b++) {
			for(int c = b+1; c < N - 2; c++) {
				for(int d = c+1; d < N - 1; d++) {
					for(int e = d+1; e < N; e++) {
						fprintf(file_pointer, "%d ", -q(a,b,c,d,e));
	}}}}}
	fprintf(file_pointer, "0\n");
	
	// symmetry breaking:
	// first, we require that p(0,2) is true, breaking reflection along the circle
	fprintf(file_pointer, "%d 0\n", p(0,2));*/

	// we require 180 degree rotational symmetry, which shouldn't change the result in theory???
	/*for(int j = 0; j < N; j++) {
		for(int i = 0; i < j-1; i++) {
			fprintf(file_pointer, "%d %d 0\n", p(i,j), -p((i + N/2) % N, (j + N/2) % N));
			fprintf(file_pointer, "%d %d 0\n", -p(i,j), p((i + N/2) % N, (j + N/2) % N));
	}}*/

  // Close the file
  fclose(file_pointer);

  printf("Data written to %s successfully.\n", filename);

  return 0; // Indicate successful execution
}
