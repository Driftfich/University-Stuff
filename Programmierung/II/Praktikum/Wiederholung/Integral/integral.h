#ifndef _INTEGRAL_H
#define _INTEGRAL_H

class NumIntegral
{
    double ug, og, sw;

    public:
        NumIntegral(double ug, double og, double sw);
        virtual double fvonx(double x) const = 0;
        void setUg(double ug);
        void setOg(double og);
        void setSw(double sw);
        double calculateArea() const;
};

class Square : public NumIntegral {
    public:
        Square(double ug, double og, double sw);
        double fvonx(double x) const override;
};

class Sqrt : public NumIntegral {
    public:
        Sqrt(double ug, double og, double sw);
        double fvonx(double x) const override;
};

#endif