#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N = 12;

int heatmap[35];

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
    fptr = fopen("samplesol.txt", "r");

    // Store the content of the file
    char myString[10000];
    int count = 0;

    // If the file exist
    if(fptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 10000, fptr)) {
            char * myPtr = strtok(myString, " ");
            while(myPtr != NULL) {

                char *endptr;
                long num = strtol(myPtr, &endptr, 10);


                if (*endptr != '\0') {
                    printf("Error: Invalid characters in string.\n");
                    if(count != 0){
                        printf("\nEdges inside: %d",count);
                        heatmap[count] ++;
                    }
                   
                    count = 0;
                } else {
                    if(num > 0 && pi((int)num) < pj((int)num)-1) {

                        printf("(%d,%d)\n", pi((int)num), pj((int)num));
                        count++;}
                }

                myPtr = strtok(NULL, " ");
            }
            
        }

    

    // If the file does not exist
    } else {
    printf("Not able to open the file.");
    }

    // Close the file
    fclose(fptr);

    for (int i = 20; i < 35; i++){
        printf("%d: %d graphs\n", i, heatmap[i]);
    }

}
