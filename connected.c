#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "bankers.c"
#define ABS(x) (x>0? x : -x) 


const int N = 25;
const char* outputName = "conn25.txt";
const char* readFile = "ind256.txt"; //file to read indecisive edges from
#define BANKERS




const uint64_t NC2 = (N * (N-1)) / 2;
bool conflict[NC2][NC2];

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

//given edges x and y in table, prints out what edges they are and relation
bool interpret(int x, int y){
    int xi = pi(x);
    int xj = pj(x);

    int yi = pi(y);
    int yj = pj(y);

    if(conflict[x-1][y-1]){
        printf("(%d, %d) & (%d, %d) cross\n", xi, xj, yi, yj);
        return true;
    }
    return false;

    // printf("(%d, %d) & (%d, %d): ", xi, xj, yi, yj);
    // printf("%s\n", conflict[x-1][y-1] ? "cross when on the same side":"cannot cross");
}

void confirm(int specific_partition){
    const int ind_edge_count = 8;
    int edges[ind_edge_count+1][2];
    edges[ind_edge_count][0]=0;
    edges[ind_edge_count][1]=2;
    //copied from partitionlabel.c
    FILE *read_ptr = fopen(readFile, "r");

            int row = 0;
            char myString[1000000];
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
                
            // If the file does not exist
            } else {
            printf("Not able to open the file.");
            }
            // Close the file
            fclose(read_ptr);

    //if two edges in my partition are on the same side, then I want to check if they cross
    for(int i = 0; i < ind_edge_count; i++){
        edges[i][1] = ((specific_partition & 1<<i) == 0) ? edges[i][1] : -edges[i][1];
    }

    for(int i = 0; i <=ind_edge_count; i++){
        for(int j = 0; j <= ind_edge_count; j++){
            if((edges[i][1] < 0 && edges [j][1] <0) || (edges[i][1] >= 0 && edges [j][1] >=0)){
                unsigned long e1 = p(ABS(edges[i][0]), edges[i][1]);
                unsigned long e2 = p(ABS(edges[j][0]), edges[j][1]);
                interpret(e1,e2);
            }
        }
    }
}


int main(){

    length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));
    

    unsigned long max = p(0,1);
    unsigned long min = p(N-1, N-2);
    assert(min < max);

    //runs through every edge pair and fills out conflict 
    //with whether being on same side causes crossing or not
    for(unsigned long e = min; e <= max; e++){
        for(unsigned long f = e+1; f <= max; f++){
          
            int ei = pi(e);
            int ej = pj(e);
            

            int fi = pi(f);
            int fj = pj(f);
            


            #ifdef BANKERS
            assert(ei < ej);
            assert(fi < fj);
            #endif

            //edges from same vertex (one of these conditions may be superfluous bc bankers)
            if(ei == fi || ei == fj || ej == fi || ej == fj){
                assert(ei != fi || ej != fj);
                assert(ei != fi || ej != fj);
                
                continue;
            } else {
                
                if((ei < fi && fi < ej && ej < fj) || (fi < ei && ei < fj && fj < ej)) {
                    conflict[e-1][f-1] = true;
                    conflict[f-1][e-1] = true;
                }
            }

        }
    }

    FILE *file_pointer;
    file_pointer = fopen(outputName, "w");

    if (file_pointer == NULL) {
        printf("Error opening the file.\n");
        return 1; // Indicate an error
    }

    printf("CONFLICT ARRAY: \n");

    for(int i = 0; i < NC2; i++){
        for(int j = 0; j < NC2; j++){
            if(conflict[i][j] == 0){
                printf(".");
            } else {
                printf("1");
            }
            fprintf(file_pointer, " %d", conflict[i][j]);
        }
        printf("\n");
        fprintf(file_pointer, "\n");
    }
    fclose(file_pointer);

    //confirm(4);

    // int c1, c2;
    // for(int i = 1; i < NC2+1; i++){
    //     if (interpret(p(0,5), i)) c1++;
    // }
    // for(int i = 1; i < NC2+1; i++){
    //     if (interpret(p(19,24), i)) c2++;   
    // }

    // printf("%d %d\n", c1, c2);

    return 0;
}