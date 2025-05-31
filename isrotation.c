#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int N = 12;

bool rotstorage[24][12][12];

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
    FILE *fptr;

    // Open a file in read mode
    fptr = fopen("k5.sol", "r");

    // Store the content of the file
    char myString[10000];
		int count;
    int curIndex;
		int curTable;

    // If the file exist
    if(fptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 10000, fptr)) {
            char * myPtr = strtok(myString, " ");
            if(myPtr!=NULL && strcmp(myPtr, "v")==0){
                while(myPtr != NULL) {
                    char *endptr;
                    long num = strtol(myPtr, &endptr, 10);

                    if (*endptr != '\0') {
                        //printf("Error: Invalid characters in string.\n");
                        if(count != 0){
                            printf("------Edges inside graph %d: %d------\n", curTable, count);
														curTable++;
                        }

                        count = 0;
                    } else {
												//in the current table, store if the edge is inside
												rotstorage[curTable][pi(abs(num))][pj(abs(num))] = (num > 0);
                        if((num > 0) && pi(num) < pj(num) - 1) count++;
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

		// search through the 24choose2 pairs of tables to see which match
		for(int a = 0; a < 24; a++) {
			// actually, first we're gonna check that a has all the properties we expect
			for(int i = 0; i < N; i++) {
				// true diagonal
				assert( rotstorage[a][i][i] );
				// true diagonal+1
				assert( rotstorage[a][i][(i+1) % N] );
				for(int j = 0; j < i; j++) {
					// symmetric
					assert( rotstorage[a][i][j] == rotstorage[a][j][i] );
			}}
			// okay, back to searching through pairs of graphs
			for(int b = 0; b < 24; b++) {
				// don't want these though
        if(a == b) continue;
				// bigMatch stores if THERE EXISTS an x with the properties we want
				bool bigMatch = false;
				bool posmatchbys[12];
				bool negmatchbys[12];
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
						bigMatch = true;
						if(match) posmatchbys[x] = true;
						if(flipMatch) negmatchbys[x] = true;
					}
				}
				if(bigMatch) {
					printf("graph %d and graph %d match by:", a, b);
					// this would mean the graphs are the exact same - because this doesn't trigger it means any 0s we see indicate a reflection!
					assert( !posmatchbys[0] );
					for(int x = 0; x < N; x++) {
						if(posmatchbys[x]) printf(" %d", x);
						if(negmatchbys[x]) printf(" %d", -x);
					}
					printf("\n");
				}
		}}
							
}
