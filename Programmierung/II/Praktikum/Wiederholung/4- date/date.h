#ifndef _DATE_H_
#define _DATE_H_

#include <iostream>

using namespace std;

class date {
    private:
        int day;
        int month;
        int year;

        void setDay(int d);
        void setMonth(int m);
        void setYear(int y);
    
    public:
        date(int d, int m, int y);
        date();
        void setDate(int d=1, int m=1, int y=2001);
        void show(ostream& os) const;

        int getDay() const {return day;};
        int getMonth() const {return month;};
        int getYear() const {return year;};

        struct tm getCurrDate() const;
        int getCurrDay() const;
        int getCurrMonth() const;
        int getCurrYear() const;
};

#endif