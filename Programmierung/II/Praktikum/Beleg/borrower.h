#ifndef _BORROWER_H
#define _BORROWER_H

#include "person.h"
#include "config.h"
#include <iostream>

using namespace std;

class Borrower: public Person {
    unsigned int limit; // Limit the amount of items one borrower can hold at the same time
    unsigned long lib_id; // Library ID
    unsigned long bow_id; // Borrower ID 

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

        void setLibId(unsigned long lib_id) {
            this->lib_id = lib_id;
        }

        void setBowId(unsigned long bow_id) {
            this->bow_id = bow_id;
        }

        // getter method
        unsigned int getLimit() const {
            return this->limit;
        }

        unsigned long getLibId() const {
            return this->lib_id;
        }

        unsigned long getBowId() const {
            return this->bow_id;
        }
}

#endif