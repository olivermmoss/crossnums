#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bankers.c"

 #define MAX(a, b) ((a) > (b) ? (a) : (b))
 #define MIN(a, b) ((a) < (b) ? (a) : (b))

//FOR USER TO DECLARE*****************

const int N = 10;

const int solutions = 172070;

bool printClasses = false;

const char* fileName = "k10nok5.sol";

//#define BANKERS


//ENDS HERE***************************

bool rotstorage[solutions][N][N];
int insideCount[solutions];
bool unique[solutions];

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

	int Nchoose2 = N * (N-1) / 2;

    // Open a file in read mode
    fptr = fopen(fileName, "r");

    // Store the content of the file
    char myString[1000000];
		int count;
    int curIndex;
		int curTable;

    // If the file exist
    if(fptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 1000000, fptr)) {
            char * myPtr = strtok(myString, " ");
            if(myPtr!=NULL && strcmp(myPtr, "v")==0){
                while(myPtr != NULL) {
                    char *endptr;
                    long num = strtol(myPtr, &endptr, 10);

                    if (*endptr != '\0') {
                        //printf("Error: Invalid characters in string.\n");
                        if(count != 0){
                            if(!printClasses) printf("------Edges inside graph %d: %d------\n", curTable, count);

							//tracks which graphs have 27 edges inside and 27 edges outside
							insideCount[curTable] = count;

							curTable++;

							
                        }
						

                        count = 0;
                    } else {
						//in the current table, store if the edge is inside
						rotstorage[curTable][pi(abs(num))][pj(abs(num))] = (num > 0);
						rotstorage[curTable][pj(abs(num))][pi(abs(num))] = (num > 0);
                        if((num > 0) && pi(num) < pj(num) - 1 && !(pi(num)==0 && pj(num)==N-1)) count++;
                    }

                    myPtr = strtok(NULL, " ");

                }
            }
        }
		//}

    // If the file does not exist
    } else {
    printf("Not able to open the file.");
    }

    // Close the file
    fclose(fptr);

	for(int i = 0; i < solutions; i++) unique[i] = true;

	int maxInsideCount = Nchoose2 - N;

	// search through the 24choose2 pairs of tables to see which match
	for(int a = 0; a < solutions; a++) {
	// but only pairs where both aren't in any classes found yet!
	if(!unique[a]) continue;

		for(int b = 0; b < solutions; b++) {
			if(!unique[b]) continue;
				
			if(insideCount[a] == maxInsideCount - insideCount[b]){
				bool bigSelf = false;
				bool posSelf[N] = {false};
				bool negSelf[N] = {false}; 

				
				for(int x = 0; x < N; x++) {
					// match is "is it a rotation (check ALL i,j)"
					bool rotSelf = true;
					// flipMatch is "is it a reflection (check ALL i,j)"
					bool flipSelf = true;
					for(int i = 0; i < N; i++) {
						for(int j = 0; j < N; j++) {
							int max = MAX(i-j, j-i);
							if(max <=1 || max ==N-1) continue;

							if(rotstorage[a][i][j] == rotstorage[b][(i+x) % 12][(j+x) % 12])
								rotSelf = false;
							if(rotstorage[a][i][j] == rotstorage[b][(12-i+x) % 12][(12-j+x) % 12])
								flipSelf = false;
					}}
					if(rotSelf || flipSelf) {
						if(!(x == 0 && !flipSelf)) bigSelf = true;
						if(rotSelf) posSelf[x] = true;
						if(flipSelf) negSelf[x] = true;
					}
				}
			
				if(bigSelf) {
					if(a != b) unique[MAX(a,b)] = false;
					if(printClasses) continue;
				
					printf("(I %d, O %d): ", a, b);
					for(int x = 0; x < N; x++) {
						if(posSelf[x]) printf(" %d", x);
						if(negSelf[x]) printf(" %d", x-N);
					}
					printf("\n");

				
				}

			}

			// bigMatch stores if THERE EXISTS an x with the properties we want
			bool bigMatch = false;
			bool posmatchbys[N] = {false};
			bool negmatchbys[N] = {false};
			for(int x = 0; x < N; x++) {
				// match is "is it a rotation (check ALL i,j)"
				bool match = true;
				// flipMatch is "is it a reflection (check ALL i,j)"
				bool flipMatch = true;
				for(int i = 0; i < N; i++) {
					for(int j = 0; j < N; j++) {
						if(rotstorage[a][i][j] != rotstorage[b][(i+x) % 12][(j+x) % 12])
							match = false;
						if(rotstorage[a][i][j] != rotstorage[b][(12-i+x) % 12][(12-j+x) % 12])
							flipMatch = false;
				}}
				if(match || flipMatch) {
					if(!(x == 0 && !flipMatch)) bigMatch = true;
					if(match) posmatchbys[x] = true;
					if(flipMatch) negmatchbys[x] = true;
				}
			}
			if(bigMatch) {
				if(a != b) unique[MAX(a,b)] = false;
				if(printClasses) continue;

				printf("(I %d, I %d):", a, b);
				for(int x = 0; x < N; x++) {
					if(posmatchbys[x]) printf(" %d", x);
					if(negmatchbys[x]) printf(" %d", x-N);
				}
				printf("\n");
			}
	}}
	
	if(!printClasses) {
		printf("unique sols: ");
		for(int i = 0; i < solutions; i++) {
			if(unique[i]) printf("%d ", i);
		}
		printf("\n");
	} else {
		for(int i = 0; i < solutions; i++) {
			if(unique[i]) {
				printf("solution %d:\nv ", i);
				for(int p = 1; p <= N*N; p++) {
					printf("%d ", rotstorage[i][pi(p)][pj(p)] ? p : -p);
				}
				printf("\n");
			}
		}
	}
}
