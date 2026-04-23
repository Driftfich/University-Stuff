#include <stdio.h>
#include <stdlib.h>
// #include <math.h>

int main() {
    char buf[128];
    long rad;
    long area;
    printf("Please enter the radius of the circle in mm: ");
    fgets(buf, sizeof(buf), stdin);
    printf("\n");
    // rad = atof(buf);
    // area = M_PI * rad * rad;
    // printf("Entered radius: %lf\n", rad);
    // printf("Circle Area: %lf\n", area);
    rad = atol(buf);
    printf("Entered radius: %ldmm\n", rad);
    area = 314 * rad * rad;
    int vorkomma = area / 100;
    int nachkomma = area % 100;
    // printf("Vorkomma: %d", vorkomma);
    // printf("Nachkomma: %d", nachkomma);
    printf("Circle area: %0d.%0d mm^2\n", vorkomma, nachkomma);
    return 0;
}