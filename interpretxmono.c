#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bankers.c"
#include <assert.h>

int N = 12;
int searchMax = (N * (N-1) * (N-2))/6;
bool inner = true;

//int heatmap[150];

unsigned long minBankers;

// p(i,j) is whether the edge btwn i and j is inside or outside.
// note that we only use this when i < j - 1, otherwise it's a useless var
// but encoding it this way is way easier lol - we just set useless vars to true
int s(int i, int j, int k) {
	// plus one is to move it from [0, N) to [1, N]
	// don't think I need to do any casting as long as N < 31
	unsigned long pos = ((1 << i) + (1 << j) + (1 << k));
	
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

// get k given p
int pk(int p) {
	assert(p > 0);

	unsigned long pos = compute(p - 1 + minBankers);
	bool foundOne = false;
	bool foundTwo = false;
	for(int i = 0; i < N; i++) {
		if(pos & 1 == 1 && foundTwo) return i;
		if(pos & 1 == 1 && foundOne) foundTwo = true;
		if(pos & 1 == 1 && !foundOne) foundOne = true;
		pos = pos >> 1;
	}
	assert(false);
	return -1;
}

//list edges inside from sample sol
int main(){
    FILE *fptr;

	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2) + (1 << N-3));

    // Open a file in read mode
    fptr = fopen("xmono.sol", "r");

    // Store the content of the file
    char myString[10000];
    int count = 0;

    // If the file exist
    if(fptr != NULL) {

		int countedVars = 0;
		
        // Read the content and print it
        while(fgets(myString, 10000, fptr)) {
            char * myPtr = strtok(myString, " ");
            if(myPtr!=NULL){
                while(myPtr != NULL) {
                    char *endptr;
                    long num = strtol(myPtr, &endptr, 10);

										if( strcmp(myPtr, "v")==0 )
										{
											countedVars = 0;
										}
	

                    if (*endptr != '\0') {
                        //printf("Error: Invalid characters in string.\n");
                        if(count != 0){
                            //printf("------Edges inside: %d------\n\n",count);
                            //heatmap[count] ++;
                        }

                        count = 0;
                    } else if(countedVars < searchMax) {
												int i = pi(abs(num));
												int j = pj(abs(num));
												int k = pk(abs(num));
												countedVars++;
												printf("saw num: %li, i = %d, j = %d, k = %d\n", num, i, j, k);

                    }

                    myPtr = strtok(NULL, " ");

                }
            }
        }


    // If the file does not exist
    } else {
    printf("Not able to open the file.");
    }

    // Close the file
    fclose(fptr);

    //for (int i = 125; i < 145; i++){
    //    printf("%d: %d graphs\n", i, heatmap[i]);
    //}

}
