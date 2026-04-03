#include <stdio.h>

void drawCircle(int r) {
    float ratio = 2.0; 
    
    for (int y = -r; y <= r; y++) {
        for (int x = -r * ratio; x <= r * ratio; x++) {
            float dx = x / ratio;
            float dist = dx * dx + y * y;

            if (dist <= r * r) {
                
                if (dx * dx + y * y < (r / 3.0) * (r / 3.0)) {
                    printf("O");  
                } else {
                    printf("*");  
                }
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