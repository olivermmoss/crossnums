#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bankers.c"
#include <assert.h>
#include <math.h>
#include <string.h>

#define POW(a, b) ((int)pow((double)a, (double)b))





//FOR USER TO DECLARE*****************

int N = 25;

const char *filename = "k6ram.cnf"; // Specify the name of the file



#define UNITS

const char *unitfile = "ind256.txt";
int ind_edge_count = 8;

int specific_partition;  // value now based on command line


//ENDS HERE***************************






unsigned long minBankers;

// p(i,j) is whether the edge btwn i and j is inside or outside.
// note that we only use this when i < j - 1, otherwise it's a useless var
// but encoding it this way is way easier lol - we just set useless vars to true
int p(int i, int j) {
	// plus one is to move it from [0, N) to [1, N]
	// don't think I need to do any casting as long as N < 31
	unsigned long pos = ((1 << i) + (1 << j));
	
	return 1 + inverse(pos) - minBankers;
}



//got this literally from the google AI output, just searched "C write to file skeleton code"
int main(int argc, char **argv) {

	if(argc == 2){
		specific_partition = (int) atoi(argv[1]);
		assert(0<= specific_partition && specific_partition < POW(2,ind_edge_count));
	} else {
		#ifdef UNITS
		fprintf(stderr, "IN UNITS MODE: EXPECTS PARTITION #");
		#endif
	}

  	FILE *file_pointer;
  
	
	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));

  // Open the file in write mode ("w"). If the file doesn't exist, it will be created.
  // If the file exists, it will be overwritten.
  file_pointer = fopen(filename, "w");

  if (file_pointer == NULL) {
    printf("Error opening the file.\n");
    return 1; // Indicate an error
  }

  int maxLen = 2;

  // Write data to the file
  uint64_t Nchoose2 = (N * (N-1)) / 2;
  uint64_t Nchoose6 = (N * (N-1))/2 * ((N-2) * (N-3))/2 * ((N-4) * (N-5)) / 180;


  uint64_t clause_num = 2*Nchoose6 + N;
  #ifdef UNITS
  	clause_num += (1+ind_edge_count);
  #endif


  fprintf(file_pointer, "p cnf %lu %lu\n", Nchoose2, clause_num); //+(maxLen-1)*2

	// adjacent edges are true:
	for(int i = 0; i < N; i++) {
		fprintf(file_pointer, "%d 0\n", p(i, (i+1) % N));
	}

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

	// PARTITION
	
	#ifdef UNITS
		fprintf(file_pointer, "%d 0\n", p(0,2));


		FILE *read_units;
		// Open a file in read mode
            read_units = fopen(unitfile, "r");

            // If the file exist
            if(read_units != NULL) {

				char myString[1000000];
				int count = 0;

                // Read the content and print it
                while(fgets(myString, 1000000, read_units)) {
					
                    char * myPtr = strtok(myString, " ");
					bool i_turn = true;
					int i_here;
					int j_here;
                    

					while(myPtr != NULL) {
						char *endptr;
						char num = (char) strtol(myPtr, &endptr, 10);

						if (*endptr != '\0') {
							assert(0<=num && num <N);
							
							if(i_turn) { 
								i_here = num;
							} else {
								j_here = num;
								if((specific_partition & 1<<count) == 0) {
									
									fprintf(file_pointer, "%d 0\n", p(i_here,j_here));

								} else {

									fprintf(file_pointer, "%d 0\n", -p(i_here,j_here));

								} 
							}
						} 

						i_turn = false;
						myPtr = strtok(NULL, " ");

                	}
					count++;
                }
                

            // If the file does not exist
            } else {
            printf("Not able to open the file.");
            }
        

            // Close the file
            fclose(read_units);
			
	#endif







// <<<<<<< HEAD
	// // require 180 deg rotational self-symmetry
	// for(int i = 0; i < N; i++) {
	// 	for(int j = 0; j < i; j++) {
	// 		fprintf(file_pointer, "%d %d 0\n", p(i,j), -p((i+N/2)%N,(j+N/2)%N));
	// 		fprintf(file_pointer, "%d %d 0\n", -p(i,j), p((i+N/2)%N,(j+N/2)%N));
	// }}

	// // alternating len2 edges 
	// for(int i = 0; i < N; i++) {
	// 	if(i%2 ==0){
	// 		fprintf(file_pointer, "%d 0\n", p(i,(i+2)%N));
	// 	} else {
	// 		fprintf(file_pointer, "%d 0\n", -p(i,(i+2)%N));
	// 	}
	// }

	// //requires roughly balanced distribution of edge of length len
	
	// for(int ind = 2; ind <= maxLen; ind ++){

	// 	int len = ind;

	// 	fprintf(file_pointer, "k %d ", N/4 *2); 
	// 	//!= N/2, e.g N=26, can't have >=13 true for both sides with symm pairs of edges
	// 	//want n/2 edges (pairs of clauses) on each side
	// 	for(int i = 0; i < N; i++) {
	// 		fprintf(file_pointer, "%d ", p(i,(i+len)%N));
	// 	}
	// 	fprintf(file_pointer, "0\nk %d ", N/4 *2);
	// 	for(int i = 0; i < N; i++) {
	// 		fprintf(file_pointer, "%d ", -p(i,(i+len)%N));
	// 	}
	// 	fprintf(file_pointer, "0\n");
	// }



	// =======
		// require 180 deg rotational self-symmetry
		/*for(int i = 0; i < N; i++) {
			for(int j = 0; j < i; j++) {
				fprintf(file_pointer, "%d %d 0\n", p(i,j), -p((i+N/2)%N,(j+N/2)%N));
				fprintf(file_pointer, "%d %d 0\n", -p(i,j), p((i+N/2)%N,(j+N/2)%N));
		}}*/
// >>>>>>> 134846f (just changed N)

  // Close the file
  fclose(file_pointer);

  printf("Data written to %s successfully.\n", filename);

  return 0; // Indicate successful execution
}
