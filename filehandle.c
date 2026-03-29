#include <stdio.h>
#include <stdlib.h>

int main() {
    
    
    FILE* fptr;

    fptr = fopen("file.txt", "w");

  
    if (fptr == NULL) 
        printf("The file is not opened.");
    else 
       printf("The created file was successful!");
    return 0;
}