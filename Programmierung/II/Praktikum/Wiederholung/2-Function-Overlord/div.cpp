#include <iostream>
#include <iomanip>

using namespace std;

int divide (int x, int y, int & rest) {
    rest = x % y;
    int result = (x-rest) / y;
    cout << "x/y = " << result << " Rest " << rest << endl;
    return result;
}

double divide(int x, int y) {
    cout << "x/y = " << (x/y) << endl;
    return x / y;
}

int main() {
    int rest;
    cout << "With constants:\t" << endl;
    int qout = divide(10, 3);

    int a=14, b=3;
    cout << "With vars:\t" << endl;
    qout = divide(a, b);

    a=14, b=3;
    cout << "With vars:\t" << endl;
    qout = divide(a+1, b+1, rest);

    return 0;
}