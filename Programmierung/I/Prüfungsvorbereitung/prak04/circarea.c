#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    char buf[128];
    double rad;
    double area;
    printf("Please enter the radius of the circle: ");
    fgets(buf, sizeof(buf), stdin);
    printf("\n");
    rad = atof(buf);
    area = M_PI * rad * rad;
    printf("Entered radius: %lf\n", rad);
    printf("Circle Area: %lf\n", area);
}