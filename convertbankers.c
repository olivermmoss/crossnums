#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "bankers.c"
#define ABS(x) (x>0? x : -x) 


const int N = 25;
const char* outputName = "edges25.txt";


const uint64_t NC2 = (N * (N-1)) / 2;
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


int main(){

    length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));
    

    unsigned long max = p(0,1);
    unsigned long min = p(N-2, N-1);
    assert(min < max);

    FILE *file_pointer;
    file_pointer = fopen(outputName, "w");

    if (file_pointer == NULL) {
        printf("Error opening the file.\n");
        return 1; // Indicate an error
    }

    for(int i = max; i >= min; i--){
        fprintf(file_pointer, "(%d, %d) - %d\n", pi(i), pj(i), i);
    }

    fclose(file_pointer);


    return 0;
}