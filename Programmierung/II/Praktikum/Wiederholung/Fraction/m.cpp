#include "frac.h"
#include <iostream>

using namespace std;

int main() {
    Fraction f1, f2(23,3), f3(2);
    f1 = f2;
    f1 /= f3;
    cout << f2 << " * " << f3 << "= " << (f2*f3) << endl;
    cout << f2 << " / " << f3 << "= " << f1 << endl;
}