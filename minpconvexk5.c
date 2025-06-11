#include <stdio.h>
#include <stdlib.h>
#include "bankers.c"

// number of vertices
int N = 14;
unsigned long Nchoose5 = (N * (N-1) * (N-2) * (N-3) * (N-4)) / 120;

// searching for at most/exactly this many pseudoconvex pentagons
int k = 12;
bool exact = false;

int minBankers;

// p(i,j) is whether the edge btwn i and j is inside or outside.
// note that we only use this when i < j - 1, otherwise it's a useless var
// but encoding it this way is way easier lol - we just set useless vars to true
int p(int i, int j) {
	// plus one is to move it from [0, N) to [1, N]
	return i * N + j + 1;
}

// get i given p
int pi(int p) {
	return (p - 1) / N;
}

// get j given p
int pj(int p) {
	return (p - 1) % N;
}

// q(a,b,c,d,e) is whether those vtxs are a pseudo-convex pentagon
int q(int a, int b, int c, int d, int e) {
	// don't think I need to do any casting as long as N < 31
	unsigned long pos = ((1 << a) + (1 << b) + (1 << c) + (1 << d) + (1 << e));
	
	return 1 + N*N + inverse(pos) - minBankers;
}

//got this literally from the google AI output, just searched "C write to file skeleton code"
int main() {
	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2) + (1 << N-3) + (1 << N-4) + (1 << N-5));

	// testing choose (I don't like the outputs but they work???)
	/*printf("%lu\n", Nchoose5);
	printf("%lu\n", choose(N, 5));
	printf("%lu\n", choose(13, 5));
	printf("%d\n", (13*12*11*10*9)/120);*/

  FILE *file_pointer;
  const char *filename = "mink5.knf"; // Specify the name of the file

  // Open the file in write mode ("w"). If the file doesn't exist, it will be created.
  // If the file exists, it will be overwritten.
  file_pointer = fopen(filename, "w");

  if (file_pointer == NULL) {
    printf("Error opening the file.\n");
    return 1; // Indicate an error
  }

  // Write data to the file
	int Nchoose2 = (N * (N-1)) / 2;
	unsigned long Nclauses = (2*N + 2*Nchoose2 + 22*Nchoose5 + 1 + 1);
	if(exact) Nclauses++;

  fprintf(file_pointer, "p knf %lu %lu\n", N*N + Nchoose5, Nclauses);
	// it's + 2*(Nchoose2 - N + 1) if we're doing symmetry  

	// first, we set useless vars to true:
	// p(i,j), is true arbitrarily when j = i or i + 1 (note this takes care of (0, n-1) because of symmetry below
	for(int i = 0; i < N; i++) {
			fprintf(file_pointer, "%d 0\n", p(i,i));
			fprintf(file_pointer, "%d 0\n", p(i,(i+1) % N));
	}

	// p(i,j) = p(j,i)
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < i; j++) {
			fprintf(file_pointer, "%d %d 0\n", p(i,j), -p(j,i));
			fprintf(file_pointer, "%d %d 0\n", -p(i,j), p(j,i));
	}}

	// Now we add the clauses: want to loop through every group of 5
	for(int a = 0; a < N - 4; a++) {
		for(int b = a+1; b < N - 3; b++) {
			for(int c = b+1; c < N - 2; c++) {
				for(int d = c+1; d < N - 1; d++) {
					for(int e = d+1; e < N; e++) {
						// want q(a,b,c,d,e) = (all inside) V (all outside)
						// (all in) V (all out) => q
						fprintf(file_pointer, "%d %d %d %d %d %d 0\n", q(a,b,c,d,e), p(a,c), p(b,d), p(c,e), p(a,d), p(b,e));
						fprintf(file_pointer, "%d %d %d %d %d %d 0\n", q(a,b,c,d,e), -p(a,c), -p(b,d), -p(c,e), -p(a,d), -p(b,e));
						// the harder one: q => (all in) V (all out)
						// it adds 20 clauses!  yikes!
						// I got these by just expanding out the propositions until they were in CNF - don't actually have representation in mind
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,c), -p(b,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,c), -p(c,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,c), -p(a,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,c), -p(b,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,d), -p(a,c));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,d), -p(c,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,d), -p(a,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,d), -p(b,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(c,e), -p(a,c));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(c,e), -p(b,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(c,e), -p(a,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(c,e), -p(b,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,d), -p(a,c));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,d), -p(b,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,d), -p(c,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(a,d), -p(b,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,e), -p(a,c));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,e), -p(b,d));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,e), -p(c,e));
						fprintf(file_pointer, "%d %d %d 0\n", -q(a,b,c,d,e), p(b,e), -p(a,d));
	}}}}}

	// finally, we add the knf clauses:

	if(exact) {
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
	fprintf(file_pointer, "%d 0\n", p(0,2));

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
