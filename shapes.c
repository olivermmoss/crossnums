#include <stdio.h>
#include <stdlib.h>
#include "bankers.c"

int N = 7;

int main() {

	for(int a = 0; a < N - 4; a++) {
		for(int b = a+1; b < N - 3; b++) {
			for(int c = b+1; c < N - 2; c++) {
				for(int d = c+1; d < N - 1; d++) {
					for(int e = d+1; e < N; e++) {
						printf("shape for %d, %d, %d, %d, %d: \n", a,b,c,d,e);
						// j is row index
						for(int j = 2; j < N; j++) {
							// i is col index
							for(int i = 0; i < j-1; i++) {
								if((i==a && j==c) || (i==b && j==d) || (i==c && j==e) || (i==a && j==d) || (i==b && j==e)) {
									printf("#");
								} else {
									if(j == N-1 && i == 0) {
										printf(" ");
									} else {
										printf(".");
									}
								}
							}
							printf("\n");
						}
	}}}}}	

	return 0;
}
