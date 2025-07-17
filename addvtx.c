#include <stdio.h>
#include <stdlib.h>
#include "bankers.c"
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>

//-----------------------------------------------------

#define BANKERS

const int N = 25;

const char* k25sfile = "allk25s.sol";

//existing vertex at pos is moved up to make space for added vertex
//e.g. pos = 4 means that the old vertex 4 becomes 5 and (3,4) becomes (3,5)
//ranges in [0,N)
int pos = 0;



//assuming number of inside edges on added vertex are in the range of [roughlyL, roughlyR]
int roughlyL = 18;
int roughlyR = 23;

//-----------------------------------------------------

uint64_t Nchoose2 = (N * (N-1)) / 2;
uint64_t Nchoose6 = (N * (N-1))/2 * ((N-2) * (N-3))/2 * ((N-4) * (N-5)) / 180;


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
		fprintf(stderr, "UHOH! %d", p);
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

int main(){

	FILE* fptr;

	fptr = fopen(k25sfile, "r");

	char myString[100000];

	int graphOnLine;
	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));

	

	if(fptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 100000, fptr)) {
            char *myPtr = strtok(myString, " ");
            if(myPtr!=NULL){
				assert(strcmp(myPtr, "v")==0);
				myPtr = strtok(NULL, " ");
				graphOnLine++;

				bool oldGraph [N][N];

                while(myPtr != NULL) {
                    char *endptr;
                    long num = strtol(myPtr, &endptr, 10);

                    if (*endptr != '\0') {
                        //printf("Error: Invalid characters in string.\n");
                    } else {
						//in the current table, store if the edge is inside
						int i = pi(abs(num));
						int j = pj(abs(num));

						assert(0<=i && i<N);
						assert(0<=j && j<N);

						oldGraph [i][j] = num > 0;
                    }

                    myPtr = strtok(NULL, " ");

                }

				//at this point we should've completed storing all the old graph's edges
				//now we check all possible combinations of inside edges for the new vertex

				//ignore self-edge and neighbors so N-2 togglable edges, represented as binary string
				for(uint32_t edges = 0; edges < (1 << (N-2)); edges++) {

					//checks for roughly evenly distributed edges
					int inside_counter;

					for(int d = 0; d < N-2; d++){
						if((edges >> d) & 1) inside_counter++;
					}
					
					if(inside_counter<roughlyL || inside_counter > roughlyR) break;


					//if we get here there are roughly evenly distributed edges on each side for added vertex
					bool newGraph[N+1][N+1];
					// set the values of the new graph
					for(int x = 0; x < N+1; x++) {
						for(int y = x+1; y < N+1; y++) { //NOTE THAT THIS IS NOT A SYMMETRIC GRAPH!!
							if(x != pos && y != pos) {
								newGraph[x][y] = oldGraph[x < pos ? x : x-1][y < pos ? y : y-1];

							// if we get here, our edge connects to the added vertex
							} else if(x == pos && y > x+1) {
								newGraph[x][y] = (edges >> (y-3)) & 1;
							} else if(x != pos && y > x+1) {
								newGraph[x][y] = (edges >> x) & 1;
							} else {
								//self-edge or neighbor
								newGraph[x][y] = true;
							}
					}}

					bool noPCH = true;
					for(int a = 0; a < N - 4; a++) {
						for(int b = a+1; b < N - 3; b++) {
							for(int c = b+1; c < N - 2; c++) {
								for(int d = c+1; d < N - 1; d++) {
									for(int e = d+1; e < N; e++) {
										for(int f = e+1; f < N+1; f++){
											if (noPCH && newGraph[a][c] && newGraph[a][d] && newGraph[a][e] 
												&& newGraph[b][d] && newGraph[b][e] && newGraph[b][f]
												&& newGraph[c][e] && newGraph[c][f] 
												&& newGraph[d][f])
												noPCH = false;
											if (noPCH && !newGraph[a][c] && !newGraph[a][d] && !newGraph[a][e] 
												&& !newGraph[b][d] && !newGraph[b][e] && !newGraph[b][f]
												&& !newGraph[c][e] && !newGraph[c][f] 
												&& !newGraph[d][f])
												noPCH = false;
					
					}}}}}}

					if(noPCH){
						fprintf(stderr, "Graph on line %d has no PCHs when new vertex is inserted at position %d.\n", graphOnLine, pos);
						//prints edges as numerical value of binary string but can later convert to binary and interpret
						fprintf(stderr, "Edges: %d\n", edges);
					}
				
				}


				//about to go onto next graph
            }
        }
		//}

    // If the file does not exist
    } else {
    printf("Not able to open the file.");
    }

    // Close the file
    fclose(fptr);


} 