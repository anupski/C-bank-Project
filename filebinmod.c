#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr;

    
    fptr = fopen("filebin.txt", "ab");

    if (fptr == NULL) {
        printf("The file could not be opened.\n");
        return 1;
    }

    
    fprintf(fptr, "\nThis is newly added content in file.txt!");

    printf("The file was modified successfully.\n");

    fclose(fptr); 
    return 0;
}