#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bankers.c"
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define POW(a, b) ((int)pow((double)a, (double)b))

/*  Takes in list of indecisive edges as a new txt file
    Prints out edges in specified partition in a new text file
*/


//FOR USER TO DECLARE*****************

//range from 0 to POW(2, ind_edge_count)-1
int specific_partition; //val assigned in command line

const int N = 25;

const char* readFile = "ind256.txt"; //file to read indecisive edges from

const char* writeFile = "partition256.txt"; //file to write partition 


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
int main(int argc, char **argv){

    if(argc == 2){
		specific_partition = (int) atoi(argv[1]);
	} else {
		fprintf(stderr, "IN UNITS MODE: EXPECTS PARTITION #\n");
	}


    FILE *read_ptr;
	
    //GETS INDECISIVE EDGES AND COMPILES THEM INTO AN ARRAY

        //first read to get number of indecisive edges
            // Open a file in read mode
            read_ptr = fopen(readFile, "r");

            // Store the content of the file
            char myString[1000000];
            int row;


            // If the file exist
            if(read_ptr != NULL) {

                // Read the content and print it
                while(fgets(myString, 1000000, read_ptr)) {
                    row++;
                }
                

            // If the file does not exist
            } else {
            printf("Not able to open the file.");
            }

            fclose(read_ptr);
        //
        const int ind_edge_count = row;
        int edges[ind_edge_count][2];

        assert(0 <= specific_partition && specific_partition < POW(2, ind_edge_count));
        

        // second read to get edges and store in an array
            // Open a file in read mode
            read_ptr = fopen(readFile, "r");

            row = 0;
            // If the file exist
            if(read_ptr != NULL) {

                // Read the content and print it
                while(fgets(myString, 1000000, read_ptr)) {
                    char * myPtr = strtok(myString, " ");
                    bool i_turn = true;

                    while(myPtr != NULL) {
						char *endptr;
						int num = (int) strtol(myPtr, &endptr, 10);
                        

                        assert(0<=num && num <N);
                        
                        if(i_turn) { 
                            edges[row][0] = num;
                        } else {
                            edges[row][1] = num;
                        }

						i_turn = false;
						myPtr = strtok(NULL, " ");

                	}




                    row++;

                }
                //}

            // If the file does not exist
            } else {
            printf("Not able to open the file.");
            }
        

            // Close the file
            fclose(read_ptr);
    //
    

    FILE *write_ptr;
		

	// Open the file in write mode ("w"). If the file doesn't exist, it will be created.
	// If the file exists, it will be overwritten.
	write_ptr = fopen(writeFile, "w");

   for(int i = 0; i < ind_edge_count; i++){
        if((specific_partition & 1<<i) == 0){
            fprintf(write_ptr, "(%d, %d)\n", edges[i][0], edges[i][1]);
        } else {
            fprintf(write_ptr, "-(%d, %d)\n", edges[i][0], edges[i][1]);
        }
    }

	fclose(write_ptr);
}
