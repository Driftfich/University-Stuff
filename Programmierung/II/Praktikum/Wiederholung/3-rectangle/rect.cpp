#include <iostream>
#include <iomanip>
#include "rect.h"

using namespace std;

void Rect::draw() const {
    // set y height
    for (int i=0; i<getPosy(); i++) cout << endl;

    for (int row=0; row<getHeight(); row++) {
        for (int i=0; i<getPosx(); i++) cout << " ";
        for (int col=0; col<getWidth(); col++) {
            cout << '0';
        }
        cout << endl;
    }
    cout << endl;
}

void Rect::print() const {
    cout << "Type: Rect" << "\n============";
    cout << "\n\tWidth: " << getWidth() << "\tHeigth: " << getHeight();
    cout << "\n\tPosx: " << getPosx() << "\tPosy: " << getPosy() << endl;
}

ostream& operator<<(ostream& os, const Rect& r) {
    r.print();
    r.draw();
    return os;
}

int main() {
    Rect r1 = Rect(2, 2, 1, 1);
    cout << r1;
}
