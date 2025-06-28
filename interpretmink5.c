#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bankers.c"

int N = 14;

int Nchoose5 = (N * (N-1) * (N-2) * (N-3) * (N-4)) / 120;
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

//list edges inside from sample sol
int main(){
	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2) + (1 << N-3) + (1 << N-4) + (1 << N-5));

    FILE *fptr;

    // Open a file in read mode
    fptr = fopen("6pentsk13.sol", "r");

    // Store the content of the file
    char myString[10000];
    int count = 0;

    // If the file exist
    if(fptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 10000, fptr)) {
            char * myPtr = strtok(myString, " ");
            if(myPtr!=NULL && strcmp(myPtr, "v")==0){
                while(myPtr != NULL) {
                    char *endptr;
                    long num = strtol(myPtr, &endptr, 10);

										//printf(myPtr);
										//printf("num = %li\n", num);

                    if (*endptr != '\0' && false) {
                        printf("Error: Invalid characters in string.\n");
                        if(count != 0){
                            printf("------Edges inside: %d------\n\n",count);
                        }

                        count = 0;
                    } else {
												if(abs(num) <= N*N) {
													// num represents an edge being inside/outside
                        	if(num < 0 && pi((int)-num) < pj((int)-num)-1) {
                            printf("%d/%d,", pi((int)-num), pj((int)-num));
													}
												}
												if(N*N < abs(num) && abs(num) <= N*N + Nchoose5) {
													// num represents set of 5 vtxs!
													if(abs(num) == N*N + 1) printf("\npseudo-convex pentagons:\n");
													if(num > 0) {
														printf("%li: ", abs(num));
														int pos = compute(abs(num) - N*N - 1 + minBankers);
														//output(pos);
														for(int i = 0; i < N; i++) {
															if(pos & 1 == 1) printf("%d ", i);
															pos = pos >> 1;
														}
														printf("\n");
													}
												}
												/*if(abs(num) > N+N + Nchoose5) {
													fclose(fptr);
													return 0;
												}*/

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

}
