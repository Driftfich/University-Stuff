#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>

using namespace std;

double sin(double rad) {
    double y, summand;
    y=rad;
    summand = rad;

    int count = 1;

    do {
        summand = -1 * summand * rad * rad / (2*count * (2 * count + 1));
        y+=summand;
        count++;
    } while (fabs(summand)>0.000009);
    return y;
}

double sin(int degree) {
    double rad = degree * (M_PI / 180.0);
    return sin(rad);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Invalid parameter count." << endl;
        return -1;
    }

    double res;
    if (strstr(argv[1], ".") != NULL) {
        double arc = atof(argv[1]);
        res = sin(arc);
        cout << "Sinus(" << arc << ") = " << res << endl;
    }
    else {
        int degree = atoi(argv[1]);
        res = sin(degree);
        cout << "Sinus(" << degree << ") = " << res << endl;
    }

    return 0;
}