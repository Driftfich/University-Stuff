#ifndef _JOB_H
#define _JOB_H

#include <iostream>
#include <string>
#include "date.h"

using namespace std;

class Job : public Date {
    string job;
    string name;

    public:
        Job(const char* job, const char* name, int d, int m, int y);
        ~Job();
        friend ostream& operator<<(ostream& os, const Job& j);
};

#endif