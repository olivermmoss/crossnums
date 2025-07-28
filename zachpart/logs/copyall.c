#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <string.h>

const char *outputName = "revallk25s.sol";
int max = 255;

int main(){

    // Open a file in read mode
    FILE *final = fopen(outputName, "w");
    char buf[30];

    for(int i = 0; i <= max; i++){
    
    snprintf(buf, 30, "k6ram_%d.cnf.log", i); // puts string into buffer

    FILE *fptr = fopen(buf, "r");
    printf("%s\n", buf);

    // Store the content of the file
    char myString[1000000];

    // If the file exist
    if(fptr != NULL) {

        // Read the content and print it
        while(fgets(myString, 1000000, fptr)) {
            fprintf(final, "%s", myString);
        }
    }

    // If the file does not exist
    else {
    printf("Not able to open the file.");
    }

   

}
    fclose(final);
}