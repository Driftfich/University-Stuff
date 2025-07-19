#include "figur.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& os, const Figur& other) {
    other.print(os);
    return os;
}