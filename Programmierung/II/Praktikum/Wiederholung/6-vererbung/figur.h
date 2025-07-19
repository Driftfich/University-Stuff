#ifndef _FIGUR_H_
#define _FIGUR_H_

#include <iostream>

using namespace std;

class Figur {
    
    public:
        Figur() {};
        virtual ~Figur() {};
        virtual double umfang() const=0;
        virtual double flaeche() const=0;
        virtual void print(ostream& os) const {os << "Figur ohne Eigenschaften" << endl;};
        friend ostream& operator<<(ostream& os, const Figur& other);
};

class Kreis : public Figur {
    double r;

    public:
        Kreis(double r) : Figur() {this->r = r;};
        double umfang() const override {return 2 * 3.14 * r;};
        double flaeche() const override {return 3.14 * r * r;};
        void print(ostream& os) const override {
            os << "Kreis" << endl << "=========" << endl << "\tUmfang: " << umfang() << endl << "\tFlächeninhalt: " << flaeche() << endl; 
        };
};

class Rechteck : public Figur {
    double a;
    double b;

    public:
        Rechteck(double a, double b) : Figur() {this->a = a; this->b = b;};
        double umfang() const override {return 2 * (a+b);};
        double flaeche() const override {return a*b;};
        void print(ostream& os) const override {
            os << "Rechteck" << endl << "=========" << endl << "\tUmfang: " << umfang() << endl << "\tFlächeninhalt: " << flaeche() << endl; 
        };
};

#endif