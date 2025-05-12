#ifndef _BORROWER_H
#define _BORROWER_H

#include "person.h"
#include "config.h"
#include <iostream>

using namespace std;

class Borrower: public Person {
    unsigned int limit; // Limit the amount of items one borrower can hold at the same time

    public:
        // constructor with member initializer list
        Borrower(unsigned long id, const std::string& fname, const std::string& lname, const std::string& ename,
                 const std::chrono::year_month_day birthday, Gender gender, const std::string& note,
                 const std::string& location, const std::string& email, const std::string& tel,
                unsigned int limit = MAX_ITEMS_PER_BORROWER_DEFAULT)
            : Person(id, fname, lname, ename, birthday, gender, note, location, email, tel) {
            this->limit = limit;
        }

        // copy constructor
        Borrower(const Borrower& other): Person(other) {
            this->limit = other.limit;
        }

        // copy assignment operator
        Borrower& operator=(const Borrower& other) {
            if (this != &other) {
                Person::operator=(other); // Call the base class assignment operator
                this->limit = other.limit;
            }
            return *this;
        }

        // destructor
        ~Borrower() {
            // Destructor
        }

        // setter method
        void setLimit(unsigned int limit) {
            if (MAX_ITEMS_PER_BORROWER_HARD != -1) {
                this->limit = min(limit, MAX_ITEMS_PER_BORROWER_HARD);
            } else {
                this->limit = limit;
            }
        }

        // getter method
        unsigned int getLimit() const {
            return this->limit;
        }
}

#endif