#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bankers.c"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Takes in a heatmap array printed as a txt file 
   Writes all the indecisive edges to a new txt file
*/


//FOR USER TO DECLARE*****************
const int N = 25;

const int solutions = 256;

const char* fileName = "hm256.txt"; //file to read heatmap from

const char *filename = "ind256.txt"; //file for indecisive edges to be printed out

#define BANKERS


//ENDS HERE***************************

int heatmap[N][N];

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
    FILE *hm_ptr;
	FILE *ind_edges_ptr;
		

	// Open the file in write mode ("w"). If the file doesn't exist, it will be created.
	// If the file exists, it will be overwritten.
	ind_edges_ptr = fopen(filename, "w");

	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));

    // Open a file in read mode
    hm_ptr = fopen(fileName, "r");

    // Store the content of the file
    char myString[1000000];
	int curRow;
	int curCol;


    // If the file exist
    if(hm_ptr != NULL && ind_edges_ptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 1000000, hm_ptr)) {
            char * myPtr = strtok(myString, " ");
            if(myPtr!=NULL){
                while(myPtr != NULL) {
                    char *endptr;
                    long num = strtol(myPtr, &endptr, 10);

                    if (*endptr != '\0') {
                        //printf("Error: Invalid characters in string.\n");

						//im sure there's a prettier fix for this but i cannot be bothered
						//to read up documentation on file reader so this is a patch fix
						if (curCol == N-1 && 0 < num && num < solutions){
							if(curRow < curCol)
								fprintf(ind_edges_ptr, "(%d, %d)\n", curRow, curCol);	
						}
						curCol = 0;
                    
                    } else {
						
						if(0 < num && num < solutions){
							if(curRow < curCol)
							fprintf(ind_edges_ptr, "(%d, %d)\n", curRow, curCol);
						}
						curCol++;
                    }

                    myPtr = strtok(NULL, " ");

                }
            }
			curRow++;
        }
		//}

    // If the file does not exist
    } else {
    printf("Not able to open the file.");
    }

    // Close the file
    fclose(hm_ptr);
	fclose(ind_edges_ptr);
}
