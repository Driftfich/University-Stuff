#include "integral.h"
#include <cmath>
#include <iostream>

using namespace std;

NumIntegral::NumIntegral(double ug, double og, double sw) {
    this->ug = ug;
    this->og = og;
    this->sw = sw;
}

void NumIntegral::setOg(double og) {this->og = og;}
void NumIntegral::setUg(double ug) {this->ug = ug;}
void NumIntegral::setSw(double sw) {this->sw = sw;}

double NumIntegral::calculateArea() const {
    double area;
    for (double x=ug; x < og-sw; x+=sw) {
        area += (fvonx(x) + fvonx(x+sw)) * sw / 2;
    }
    return area;
}


Square::Square(double ug, double og, double sw) : NumIntegral(ug, og, sw) {

}

double Square::fvonx(double x) const {
    return x * x;
}

Sqrt::Sqrt(double ug, double og, double sw) : NumIntegral(ug, og, sw) {

}

double Sqrt::fvonx(double x) const {
    return sqrt(x);
}

int main() {
    Square i1 = Square(0, 1, 0.0001);
    cout << i1.calculateArea() << endl;
    return 0;
}