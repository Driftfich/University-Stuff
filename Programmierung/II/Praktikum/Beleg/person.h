// Franz Rehschuh (s88216)
// Class definition for the Person (abstract) class and child classes
// TODO: Implement class for Tel number and Location

// Return values
// return -1 <=> error
// return 0 <=> fine

#infdef _PERSON_H
#define _PERSON_H

#include <iostream>
#include <string>
#include <chrono>

#include "config.h"



enum class Gender {
    female,
    male,
    diverse,
    unknown
};

class Person {
    unsigned long id;
    std::string fname;
    std::string lname;
    std::string ename;
    std::chrono::year_month_day birthday;
    Gender gender;
    std::string note;
    std::location;
    std::email;
    std::tel;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setFname(const std::string& fname);
        int setLName(const std::string& lname);
        int setEname(const std::string& ename);
        int setBirthday(const std::chrono::year_month_day birthday);
        int setGender(const Gender gender);
        int setNote(const std::string& note);
        int setLocation(const std::string& note);
        int setEmail(const std::string& note);
        int setTel(const std::string& note);
}

class Borrower: public Person {
    int limit; // LImit the amount of items one borrower can hold at the same time

    public:
        // constructor

}

#endif