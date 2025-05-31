#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int N = 40;

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

//got this literally from the google AI output, just searched "C write to file skeleton code"
int main() {
  FILE *file_pointer;
  const char *filename = "k6ram.cnf"; // Specify the name of the file

  // Open the file in write mode ("w"). If the file doesn't exist, it will be created.
  // If the file exists, it will be overwritten.
  file_pointer = fopen(filename, "w");

  if (file_pointer == NULL) {
    printf("Error opening the file.\n");
    return 1; // Indicate an error
  }

  // Write data to the file
  uint64_t Nchoose2 = (N * (N-1)) / 2;
  uint64_t Nchoose6 = (N * (N-1))/2 * ((N-2) * (N-3))/2 * ((N-4) * (N-5)) / 180;
//   printf("%d %d %d %d\n", N-2, N-3, N-4, N-5);
//   printf("%ld\n",(uint64_t) (N * (N-1) * (N-2) * (N-3) * (N-4) * (N-5)));
//   printf("NC2: %ld\n", Nchoose2);
//   printf("NC6: %ld\n", Nchoose6);
//   printf("fin: %lu\n", ((uint64_t)N + (uint64_t)(N-1) + Nchoose2 + 2*Nchoose6 + 2));
  fprintf(file_pointer, "p cnf %d %lu\n", N*N, ((uint64_t)N + (uint64_t)(N-1) + Nchoose2 + 2*Nchoose6 + 2));
  
	// first, we set useless vars to true:
	// p(i,j), is true arbitrarily when j <= i + 1
	for(int i = 0; i < N; i++) {
		for(int j = 0; j <= i + 1; j++) {
			if(j < N) fprintf(file_pointer, "%d 0\n", p(i,j));
	}}

	// Now we add the clauses: want to loop through every group of 5
	for(int a = 0; a < N - 5; a++) {
		for(int b = a+1; b < N - 4; b++) {
			for(int c = b+1; c < N - 3; c++) {
				for(int d = c+1; d < N - 2; d++) {
					for(int e = d+1; e < N - 1; e++) {
                        for(int f = e+1; f < N; f++) {
						// at least one diagonal edge must be inside
						fprintf(file_pointer, "%d %d %d %d %d %d %d %d %d 0\n", p(a,c), p(a,d), p(a,e), p(b,d), p(b,e), p(b,f), p(c,e), p(c,f), p(d,f));
						// at least one diagonal edge must be outside
						fprintf(file_pointer, "%d %d %d %d %d %d %d %d %d 0\n", -p(a,c), -p(a,d), -p(a,e), -p(b,d), -p(b,e), -p(b,f), -p(c,e), -p(c,f), -p(d,f));
    }}}}}}

	// symmetry breaking:
	// first, we require that p(0,2) is true, breaking reflection along the circle
	fprintf(file_pointer, "%d 0\n", p(0,2));
	fprintf(file_pointer, "%d 0\n", p(0,N-1));

  // Close the file
  fclose(file_pointer);

  printf("Data written to %s successfully.\n", filename);

  return 0; // Indicate successful execution
}
