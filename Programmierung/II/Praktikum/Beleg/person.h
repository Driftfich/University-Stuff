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
    QString fname;
    QString lname;
    QString ename;
    std::chrono::year_month_day birthday;
    Gender gender;
    QString note;
    std::location;
    std::email;
    std::tel;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setFname(const QString& fname);
        int setLName(const QString& lname);
        int setEname(const QString& ename);
        int setBirthday(const std::chrono::year_month_day birthday);
        int setGender(const Gender gender);
        int setNote(const QString& note);
        int setLocation(const QString& note);
        int setEmail(const QString& note);
        int setTel(const QString& note);

        // getter methods
        unsigned long getId() const {return this->id;}
        QString getFname() const {return this->fname;}
        QString getLname() const {return this->lname;}
        QString getEname() const {return this->ename;}
        std::chrono::year_month_day getBirthday() const {return this->birthday;}
        Gender getGender() const {return this->gender;}
        QString getNote() const {return this->note;}
        QString getLocation() const {return this->location;}
        QString getEmail() const {return this->email;}
        QString getTel() const {return this->tel;}

        // constructor using setters
        Person(unsigned long id, const QString& fname, const QString& lname, const QString& ename,
                const std::chrono::year_month_day birthday, Gender gender, const QString& note,
                const QString& location, const QString& email, const QString& tel) {
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
        virtual ~Person();
}



#endif