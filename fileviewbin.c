#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr;
    char c;


    fptr = fopen("filebin.txt", "rb");

    if (fptr == NULL) {
        printf("The file could not be opened.\n");
        return 1; 
    }

    printf("Contents of file.txt:\n");

    
    while ((c = fgetc(fptr)) != EOF) {
        putchar(c);
    }

    fclose(fptr); 
    return 0;
}