#ifndef _BORROWER_H
#define _BORROWER_H

#include <QDate>
#include <iostream>
#include <algorithm>
#include "person.h"
#include "config.h"

using namespace std;

class Borrower: public Person {
    unsigned int limit; // Limit the amount of items one borrower can hold at the same time
    unsigned long bow_id; // Borrower ID 

    public:
        // constructor with member initializer list
        Borrower(unsigned long id, const QString& fname, const QString& lname, const QString& ename,
                 const QDate& birthday, const QString& gender, const QString& note,
                 const QString& location, const QString& email, const QString& tel,
                unsigned int limit, unsigned long bow_id)
            : Person(id, fname, lname, ename, birthday, gender, note, location, email, tel) {
            setLimit(limit);
            setBowId(bow_id);
        }

        // copy constructor
        Borrower(const Borrower& other): Person(other) {
            this->limit = other.limit;
            this->bow_id = other.bow_id;
        }

        // copy assignment operator
        Borrower& operator=(const Borrower& other) {
            if (this != &other) {
                Person::operator=(other); // Call the base class assignment operator
                this->limit = other.limit;
                this->bow_id = other.bow_id;
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
                this->limit = std::min(limit, static_cast<unsigned int> (MAX_ITEMS_PER_BORROWER_HARD));
            } else {
                this->limit = limit;
            }
        }

        void setBowId(unsigned long bow_id) {
            this->bow_id = bow_id;
        }

        // getter method
        unsigned int getLimit() const {
            return this->limit;
        }

        unsigned long getBowId() const {
            return this->bow_id;
        }

        // serialization methods
        QString getSubclassType() const override { return "Borrower"; }
        QJsonObject getSubclassParams() const override;
        int loadSubclassParams(const QJsonObject& json) override;
        QJsonObject getSubclassSchema() const override;
        Borrower(const QJsonObject& json) : Person(json) {
            if (loadSubclassParams(json) != 0) {
                throw std::runtime_error("Failed to load Borrower parameters");
            }
        }

};

#endif