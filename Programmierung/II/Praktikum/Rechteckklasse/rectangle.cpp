#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include "rectangle.h"

using namespace std;

int rect::draw() {
    // what about negative positions => setw to zero and substract negative pos from size in loops
    if (this->a <= 0 || this->b <= 0) return -1;

    const char* horizontal = '─';
    const char* vertical = '|';
    const char *corner_tl = "┌";
    const char* corner_tr = "┐";
    const char* corner_bl = "└";
    const char* corner_br = "┘";

    int height = round(this->b);
    int width = round(this->a);
    int posx = round(this->posx);
    int posy = round(this->posy);

    // set y pos on console
    for (int y=0; y<posy; y++) cout << endl;

    // set x pos on console
    cout << setw(posx);

    if (height <= 0 || width < 0) return 0;

    // print out the top
    cout << corner_tl << setfill(horizontal) << setw(width * 2) << corner_tr << endl;
    // print out the vertical lines in between
    cout << setfill(" ");
    for (int y=0; y<height-1; y++) {
        cout << setw(posx) << "|" << setw(width * 2) << "|" << endl;
    }

    // print out the bottom
    cout << setw(posx) << setfill(horizontal) << corner_bl << setw(width * 2) << corner_br << endl;
}
// ┌----┐
// └----┘
