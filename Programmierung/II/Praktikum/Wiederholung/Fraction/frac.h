#ifndef _FRAC_H
#define _FRAC_H

#include <iostream>

using namespace std;

class Fraction {
    long Numerator, Denom;
    long gcf(long First, long Second);

    public:
    Fraction() {Numerator = 0; Denom = 1;};
    Fraction(long Num, long Den=1);
    ~Fraction() {};

    Fraction& operator=(const Fraction& second);
    friend Fraction operator+(const Fraction& f, const Fraction& s);
    friend Fraction operator*(const Fraction& f, const Fraction& s);
    Fraction& operator/=(const Fraction& second);
    friend ostream& operator<<(ostream& os, const Fraction& f);
};

#endif