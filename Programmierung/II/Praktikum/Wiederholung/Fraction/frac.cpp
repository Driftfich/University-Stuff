#include "frac.h"
#include <iostream>
#include <cmath>

using namespace std;

long Fraction::gcf(long First, long Second) {
    First = abs(First);
    Second = abs(Second);
    if (First < Second) {
        // First ^= Second; Second ^= First; First ^= Second;
        long tmp = Second;
        Second = First;
        First = tmp;
    }
    long r=Second;
    long f;
    do {
        Second = r;
        r = First % Second;
        f = First / Second;
        First = Second;
    } while (r != 0);
    return Second;
}

Fraction::Fraction(long Num, long Denom) {
    long split = gcf(Num, Denom);
    this->Numerator = Num / split;
    this->Denom = Denom / split;
    if (this->Denom < 0) {
        this->Denom = - this->Denom;
        this->Numerator = - this->Numerator;
    }

    if (this->Denom == 0) throw std::invalid_argument("Denominator is 0");
}

Fraction& Fraction::operator=(const Fraction& second) {
    this->Denom = second.Denom;
    this->Numerator = second.Numerator;
    return *this;
}

Fraction operator+(const Fraction& f, const Fraction& s) {
    long newDenom = f.Denom * s.Denom;
    long newNum = f.Numerator * s.Denom + s.Numerator * f.Denom;
    return Fraction(newNum, newDenom);
}

Fraction operator*(const Fraction& f, const Fraction& s) {
    long newDenom = f.Denom * s.Denom;
    long newNum = f.Numerator * s.Numerator;
    return Fraction(newNum, newDenom);
}

Fraction& Fraction::operator/=(const Fraction& second) {
    long newDenom = this->Denom * second.Numerator;
    long newNum = this->Numerator * second.Denom;
    long split = gcf(newDenom, newNum);
    this->Numerator = newNum / split;
    this->Denom = newDenom / split;
    return *this;
}

ostream& operator<<(ostream& os, const Fraction& f) {
    os << "Fraction(" << f.Numerator << "/" << f.Denom << ")";
    return os;
}