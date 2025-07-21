#include "dump.h"
#include <iostream>
#include <iomanip>


using namespace std;

void DUMP::OutLine() {
    cout << setw(4) << setfill('0') << hex << idx << ": ";

    for (int i=0; i<16; i++) {
        cout << setw(2) << setfill('0') << hex << lBuf[i] << " ";
        if ((i+1) % 4 == 0 && i != 15) {
            cout << " | ";
        }
    }

    cout << "   ";
    for (int i=0; i<16; i++) {
        char c = (char) lBuf[i];
        if (isprint(c)) {
            cout << c;
        }
        else cout << ".";
    }
    cout << endl;
}