#include "job.h"
#include "date.h"
#include <iostream>
#include <string>

Job::Job(const char* job, const char* name, int d, int m, int y) : Date(d, m, y), job(job), name(name) {

}

Job::~Job() {}

ostream& operator<<(ostream& os, const Job& j) {
    os << "Name: " << j.name << "\nJob: " << j.job << "\nBirthday: " << (Date&) j << endl;
    return os;
} 