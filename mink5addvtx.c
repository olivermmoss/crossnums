#include <stdio.h>
#include <stdlib.h>
#include "bankers.c"
#include <string.h>
#include <limits.h>
#include <stdint.h>

int minBankers2;
int minBankers5;

const int N = 12;
unsigned long Nchoose5 = (N * (N-1) * (N-2) * (N-3) * (N-4)) / 120;
unsigned long Nchoose2 = (N * (N-1)) / 2;

const int solutions = 24;

bool rotstorage[solutions][N][N];

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


//got this literally from the google AI output, just searched "C write to file skeleton code"
int main() {
	length = N;
	minBankers5 = inverse((1 << N-1) + (1 << N-2) + (1 << N-3) + (1 << N-4) + (1 << N-5));
	minBankers2 = inverse((1 << N-1) + (1 << N-2));

	FILE *fptr;

    // Open a file in read mode
    fptr = fopen("k5.sol", "r");

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

	int minPentagons = INT_MAX;
	int minPos;
	uint32_t minEdges;
	int minGraph = 0;
	bool assignment[(N+1)*(N+1)];

	// graph is which of the allsat solutions we're looking at
	for(int graph = 0; graph < solutions; graph++) {
	// where the new vertex is being inserted
	for(int pos = 0; pos < N+1; pos++) {
		// it encodes all of the edges connecting to the new vertex as a binary string
		for(uint32_t edges = 0; edges < (1 << (N-2)); edges++) {

			int curPentagons = 0;
			// symmetric matrix encoding whether edges are in or out
			bool newGraph[N+1][N+1];
			// set the values of the new graph
			for(int x = 0; x < N+1; x++) {
				for(int y = 0; y < N+1; y++) {
					if(x != pos && y != pos) {
						newGraph[x][y] = rotstorage[graph][x > pos ? x-1 : x][y > pos ? y-1 : y];
					// if we get here, our edge connects to the added vertex
					} else if(x == pos && y != pos) {
						if(y < x - 1) newGraph[x][y] = (edges >> y) & 1;
						else if(y > x + 1) newGraph[x][y] = (edges >> (y-3)) & 1;
						else newGraph[x][y] = true;
					} else if(x != pos && y == pos) {
						if(x < y - 1) newGraph[x][y] = (edges >> x) & 1;
						else if(x > y + 1) newGraph[x][y] = (edges >> (x-3)) & 1;
						else newGraph[x][y] = true;
					} else {
						newGraph[x][y] = true;
					}
			}}
				

	// want to loop through every group of 5 and check if they form a pentagon
	for(int a = 0; a < N - 3; a++) {
		for(int b = a+1; b < N - 2; b++) {
			for(int c = b+1; c < N - 1; c++) {
				for(int d = c+1; d < N; d++) {
					for(int e = d+1; e < N + 1; e++) {
						if(newGraph[a][c] && newGraph[b][d] && newGraph[c][e] && newGraph[a][d] && newGraph[b][e])
							curPentagons++;
						if(!newGraph[a][c] && !newGraph[b][d] && !newGraph[c][e] && !newGraph[a][d] && !newGraph[b][e])
							curPentagons++;
	}}}}}

	// set the minimum if stuff is satisfied
	if(curPentagons < minPentagons ) {
		minPentagons = curPentagons;
		minPos = pos;
		minEdges = edges;
		minGraph = graph;
	}
	}}}

	printf("min pentagons: %d\nmin pos: %d\nmin edges: %d\nmin graph: %d\n", minPentagons, minPos, minEdges, minGraph);

 return 0; // Indicate successful execution
}
