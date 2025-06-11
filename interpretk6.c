#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bankers.c"
#include <assert.h>

int N = 25;
int searchMax = 300;
bool inner = true;

int heatmap[150];

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

//list edges inside from sample sol
int main(){
    FILE *fptr;

	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));

    // Open a file in read mode
    fptr = fopen("k6s.sol", "r");

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
                            printf("------Edges inside: %d------\n\n",count);
                            heatmap[count] ++;
                        }

                        count = 0;
                    } else if(countedVars < searchMax) {
												int i = pi(abs(num));
												int j = pj(abs(num));
												countedVars++;
												//printf("saw num: %li, i = %d, j = %d\n", num, pi(abs(num)), pj(abs(num)));
                        if(num > 0 && i < j-1 && inner) {

                            printf("%d/%d,", i, j);
                            count++;}
												// extra stuff after !inner is to make the green lines
												if(num < 0 && i < j-1 && !inner && !(abs(i-j) < N/2-1 || N - abs(i-j) < N/2-1)) {

                            printf("%d/%d,", i, j);
                            count++;}

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

    for (int i = 125; i < 145; i++){
        printf("%d: %d graphs\n", i, heatmap[i]);
    }

}
