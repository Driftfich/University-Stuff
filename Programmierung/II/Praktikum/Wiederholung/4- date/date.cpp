#include <iostream>
#include <stdlib.h>
#include "date.h"

using namespace std;

date::date(int d, int m, int y) {
    setYear(y);
    setMonth(m);
    setDay(d);
    setMonth(getMonth());
    setDay(getDay());
}

date::date() {
    setYear(getCurrYear());
    setMonth(getCurrMonth());
    setDay(getCurrDay());
    setMonth(getMonth());
    setDay(getDay());
}

void date::setDay(int d) {
    d = abs(d);
    int mod;
    if ((getMonth() % 2 == 1 && getMonth() <= 7) || (getMonth() % 2 == 0 && getMonth() > 7)) {
        // ungerader Monat
        mod = 31;
    }
    else { // gerader Monat
        mod = 30;
    }

    day = ((d-1) % mod) + 1;
    month += d / mod;
}

void date::setMonth(int m) {
    month = ((abs(m) - 1) % 12) + 1;
    year += (abs(m)-1) / 12;
}

void date::setYear(int y) {
    year = abs(y);
}

void date::setDate(int d, int m, int y) {
    setYear(y);
    setMonth(m);
    setDay(d);
}

void date::show(ostream& os) const {
    os << getDay() << "-" << getMonth() << "-" << getYear();
}

struct tm date::getCurrDate() const {
    time_t timestamp = time(&timestamp);
    struct tm datetime;
    // -1 uses the computer's timezone setting
    datetime.tm_isdst = -1;
    datetime = *localtime(&timestamp);

    return datetime;
}

int date::getCurrDay() const {
    struct tm datetime = this->getCurrDate();
    return datetime.tm_mday;
}

int date::getCurrMonth() const {
    struct tm datetime = this->getCurrDate();
    return datetime.tm_mon+1;
}

int date::getCurrYear() const {
    struct tm datetime = this->getCurrDate();
    return datetime.tm_year+1900;
}

