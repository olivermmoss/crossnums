#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bankers.c"

#define MAX(a, b) ((a) > (b) ? (a) : (b))


//FOR USER TO DECLARE*****************
const int N = 25;


const char* fileName = "k6sno2.sol"; //file to read sols from

const char *newfile = "flipk6sno2.sol"; //file for flipped output

#define BANKERS


//ENDS HERE***************************

unsigned long minBankers;


#ifdef BANKERS

	// p(i,j) is whether the edge btwn i and j is inside or outside.
	// note that we only use this when i < j - 1, otherwise it's a useless var
	// but encoding it this way is way easier lol - we just set useless vars to true
	int p(int i, int j) {
		// plus one is to move it from [0, N) to [1, N]
		// don't think I need to do any casting as long as N < 31
		unsigned long pos = ((1 << i) + (1 << j));
		
		return 1 + inverse(pos) - minBankers;
	}

	// get i given p
	int pi(int p) {
		assert(p > 0);

		unsigned long pos = compute(p - 1 + minBankers);
		for(int i = 0; i < N; i++) {
			if(pos & 1 == 1) return i;
			pos = pos >> 1;
		}
		assert(false);
		return -1;
	}

	// get j given p
	int pj(int p) {
		assert(p > 0);

		unsigned long pos = compute(p - 1 + minBankers);
		bool foundOne = false;
		for(int i = 0; i < N; i++) {
			if(pos & 1 == 1 && foundOne) return i;
			if(pos & 1 == 1 && !foundOne) foundOne = true;
			pos = pos >> 1;
		}
		assert(false);
		return -1;
	}
#else
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
#endif


//list edges inside from sample sol
int main(){
    FILE *fptr;

	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));

    // Open a file in read mode
    fptr = fopen(fileName, "r");

    // Store the content of the file
    char myString[1000000];
		int count;
    int curIndex;
		int curTable;

	FILE *new_fileptr; //flipped solns

	// Open the file in write mode ("w"). If the file doesn't exist, it will be created.
	// If the file exists, it will be overwritten.
	new_fileptr = fopen(newfile, "w");

	if (new_fileptr == NULL || fptr == NULL) {
		printf("Error opening the file.\n");
		return 1; // Indicate an error
	}

   
	// Read the content and print it
	while(fgets(myString, 1000000, fptr)) {
		char * myPtr = strtok(myString, " ");
		if(myPtr!=NULL && strcmp(myPtr, "v")==0){
			while(myPtr != NULL) {
				char *endptr;
				long num = strtol(myPtr, &endptr, 10);

				if (*endptr != '\0') {
					fprintf(new_fileptr, " 0\nv ");
				} else {
					//in the current table, store if the edge is inside
					int i = pi(abs(num));
					int j = pj(abs(num));
					int max = MAX(i-j, j-i);

					if(max <=1 || max ==N-1 || num < 0){
						fprintf(new_fileptr, " %ld", abs(num));
					} else {
						fprintf(new_fileptr, " %ld", -(num));
					}
				}

				myPtr = strtok(NULL, " ");

			}
		}
	}
		

    // Close the files
    fclose(fptr);
	fclose(new_fileptr);
	return 0;
}
