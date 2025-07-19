#include <iostream>
#include "figur.h"

using namespace std;

int main() {
    Kreis f1 = Kreis(4);
    Rechteck f2 = Rechteck(2, 2);
    cout << f1 << f2;
    return 0;
}