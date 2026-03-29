#include <stdio.h>
#include <stdlib.h>

struct threeNum {
    int n1, n2, n3;
};

int main() {
    int n =1 ;
    
   
    struct threeNum num;
    FILE* fptr;
    fptr = fopen("binaryFile.bin", "wb");
    int flag = 0;
    num.n1 = n;
    num.n2 = 5 * n;
    num.n3 = 5 * n + 1;
    
    
    flag = fwrite(&num, sizeof(struct threeNum), 1,
                  fptr);

   
    if (!flag)
        printf("Write Operation Failure");
    else
        printf("Write Operation Successful");
    fclose(fptr);
    return 0;
}