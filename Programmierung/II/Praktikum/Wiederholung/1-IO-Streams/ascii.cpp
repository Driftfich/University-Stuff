#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    int val;
    for (int row = 0; row < 32; row++) {
        cout << "| ";
        for (int col = 0; col < 4; col++) {
            val = col * 32 + row;
            cout << setw(3) << setfill('0') << dec << val << " ";
            cout << setw(2) << setfill('0') << hex << val << " ";
            char c = (char) val;
            if (!isprint(c)) c = '.';
            cout << setw(2) << setfill(' ') << c << " | ";
        }
        cout << endl;
    }
}