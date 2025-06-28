#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int N = 26;

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

// pent(a,b) is whether vtx b is in pentagon a
int pent(int a, int b) {
	return a * N + b + N*N + 1;
}

//got this literally from the google AI output, just searched "C write to file skeleton code"
int main() {
  FILE *file_pointer;
  const char *filename = "k6ram.knf"; // Specify the name of the file

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
  uint64_t Nchoose5 = (N * (N-1))/2 * ((N-2) * (N-3))/2 * ((N-4)) / 30;
//   printf("%d %d %d %d\n", N-2, N-3, N-4, N-5);
//   printf("%ld\n",(uint64_t) (N * (N-1) * (N-2) * (N-3) * (N-4) * (N-5)));
//   printf("NC2: %ld\n", Nchoose2);
//   printf("NC6: %ld\n", Nchoose6);
//   printf("fin: %lu\n", ((uint64_t)N + (uint64_t)(N-1) + Nchoose2 + 2*Nchoose6 + 2));
  fprintf(file_pointer, "p knf %d %lu\n", N*N + N*3, (2*(uint64_t)N + 2*Nchoose2 + 2*Nchoose6 + 1 + 6 + N + 3*20*Nchoose5));
  
		// first, we set useless vars to true:
	// p(i,j), is true arbitrarily when j = i or i + 1
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

	// require 180 deg rotational self-symmetry
	/*for(int i = 0; i < N; i++) {
		for(int j = 0; j < i; j++) {
			fprintf(file_pointer, "%d %d 0\n", p(i,j), -p((i+N/2)%N,(j+N/2)%N));
			fprintf(file_pointer, "%d %d 0\n", -p(i,j), p((i+N/2)%N,(j+N/2)%N));
	}}*/

	// require that pent(a,b) is true for exactly 5 bs
	for(int a = 0; a < 3; a++) {
		fprintf(file_pointer, "k 5 ");
		for(int b = 0; b < N; b++) {
			fprintf(file_pointer, "%d ", pent(a,b));
		}
		fprintf(file_pointer, "0\n");
		fprintf(file_pointer, "k %d ", N-5);
		for(int b = 0; b < N; b++) {
			fprintf(file_pointer, "%d ", -pent(a,b));
		}
		fprintf(file_pointer, "0\n");
	}
	// require that pent(a,b) is true 0 or 1 times for each b
	for(int b = 0; b < N; b++) {
		fprintf(file_pointer, "k 2 %d %d %d 0\n", -pent(0,b), -pent(1,b), -pent(2,b));
	}

	// finally, give meaning to the pentagons!
	for(int A = 0; A < 3; A++) {
	for(int a = 0; a < N - 4; a++) {
		for(int b = a+1; b < N - 3; b++) {
			for(int c = b+1; c < N - 2; c++) {
				for(int d = c+1; d < N - 1; d++) {
					for(int e = d+1; e < N; e++) {
						// p(A,a) and ... and p(A,e) => (all in) V (all out)
						// it adds 20 clauses!  yikes!
						// I got these by just expanding out the propositions until they were in CNF - don't actually have representation in mind
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,c), -p(b,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,c), -p(c,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,c), -p(a,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,c), -p(b,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,d), -p(a,c));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,d), -p(c,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,d), -p(a,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,d), -p(b,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(c,e), -p(a,c));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(c,e), -p(b,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(c,e), -p(a,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(c,e), -p(b,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,d), -p(a,c));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,d), -p(b,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,d), -p(c,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(a,d), -p(b,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,e), -p(a,c));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,e), -p(b,d));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,e), -p(c,e));
						fprintf(file_pointer, "%d %d %d %d %d %d %d 0\n", -pent(A,a), -pent(A,b), -pent(A,c), -pent(A,d), -pent(A,e), p(b,e), -p(a,d));
	}}}}}}


  // Close the file
  fclose(file_pointer);

  printf("Data written to %s successfully.\n", filename);

  return 0; // Indicate successful execution
}
