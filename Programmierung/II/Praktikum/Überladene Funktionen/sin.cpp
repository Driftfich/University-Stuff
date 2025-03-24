#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

double sin(double x) {
    double y, summand;
    y=x;
    summand = x;
    int count=1;
    do {
        summand = -1 * summand * x * x / (2*count *(2*count + 1));
        y+=summand;
        count ++;
    } while (fabs(summand)>0.000009);
    return y;
}

double sin(int x) {
    double rad = x * (M_PI / 180.0);
    return sin(rad);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Invalid cmd input.\nUsage:\t" << argv[0] << " <double|int>" << endl;
        return -1;
    }

    double res;
    if (strstr(argv[1], ".") != NULL) {
        double arc = atof(argv[1]);
        res = sin(arc);
        cout << "Sinus(" << arc << ")" << "=" << res << endl;
    }
    else {
        int angle = atoi(argv[1]);
        res = sin(angle);
        cout << "Sinus(" << angle << "°)" << "=" << res << endl;
    }

    return 0;
}