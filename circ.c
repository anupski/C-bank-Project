#include <stdio.h>

void drawCircle(int r) {

    float ratio = 2.0; 
    
   
    for (int y = -r; y <= r; y++) {
        for (int x = -r * ratio; x <= r * ratio; x++) {
           
            float dx = x / ratio;
            if (dx * dx + y * y <= r * r) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    int radius;
    printf("Enter radius: ");
    scanf("%d", &radius);
    
    drawCircle(radius);
    return 0;
}