#include <iostream>
#include "date.h"
#include "person.h"

using namespace std;

int main(int argc, char* argv[]) {
    // date d1 = date(18, 7, 2025);
    // date d2 = date();
    // d1.show(cout);
    // cout << endl;
    // d2.show(cout);
    // cout << endl;

    Person p1 = Person("Franz", "Rehschuh", 5, 12, 2005);
    p1.show(cout);
    return 0;
}