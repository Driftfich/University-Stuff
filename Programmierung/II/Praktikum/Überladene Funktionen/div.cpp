#include <iostream>
#include <cmath>

using namespace std;

int divide(int x, int y, int & rest) {
    rest = x % y;
    int result = (x-rest) / y;
    cout << "Result: " << result << " Rest: " << rest << endl;
    return result;
}
double divide(int x, int y) {
    int result = (x/y);
    cout << "Result: " << result << endl;
    return result;
}


int main() {
    int rest;
    cout << "With constants:\t" << endl;
    int qout = divide(10, 3, rest);

    int a=14, b=3;
    cout << "With vars:\t" << endl;
    qout = divide(a, b, rest);

    a=14, b=3;
    cout << "With vars:\t" << endl;
    qout = divide(a+1, b+1, rest);

    return 0;
}