#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bankers.c"
#include <assert.h>

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
	length = N;
	minBankers = inverse((1 << N-1) + (1 << N-2));

	int zachNums[] = {31,20,8,41,6,18,29,50,32,42,21,9};
						
	for(int i = 0; i < 12; i++) {
    											printf("%d: %d %d\n", zachNums[i], pi(zachNums[i]), pj(zachNums[i]));
													}
	}


