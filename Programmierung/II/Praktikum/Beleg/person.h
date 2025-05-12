// Franz Rehschuh (s88216)
// Class definition for the Person (abstract) class and child classes
// TODO: Implement class for Tel number and Location

// Return values
// return -1 <=> error
// return 0 <=> fine

#ifndef _PERSON_H
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

        // getter methods
        unsigned long getId() const {return this->id;}
        std::string getFname() const {return this->fname;}
        std::string getLname() const {return this->lname;}
        std::string getEname() const {return this->ename;}
        std::chrono::year_month_day getBirthday() const {return this->birthday;}
        Gender getGender() const {return this->gender;}
        std::string getNote() const {return this->note;}
        std::string getLocation() const {return this->location;}
        std::string getEmail() const {return this->email;}
        std::string getTel() const {return this->tel;}

        // constructor using setters
        Person(unsigned long id, const std::string& fname, const std::string& lname, const std::string& ename,
                const std::chrono::year_month_day birthday, Gender gender, const std::string& note,
                const std::string& location, const std::string& email, const std::string& tel) {
            setId(id);
            setFname(fname);
            setLname(lname);
            setEname(ename);
            setBirthday(birthday);
            setGender(gender);
            setNote(note);
            setLocation(location);
            setEmail(email);
            setTel(tel);
        }

        // copy constructor
        Person(const Person& other);

        // copy assignment operator
        Person& operator=(const Person& other);

        // destructor
        ~Person();
}



#endif