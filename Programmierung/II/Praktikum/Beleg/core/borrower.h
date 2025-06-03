#ifndef _BORROWER_H
#define _BORROWER_H

#include <QDate>
#include <iostream>
#include <algorithm>
// #include "person.h"
#include "config.h"

using namespace std;

class Borrower {
    unsigned int limit; // Limit the amount of items one borrower can hold at the same time
    unsigned long bow_id; // Borrower ID 

    public:
        // constructor with member initializer list
        Borrower(unsigned int limit) {
            setLimit(limit);
        }

        // copy constructor
        Borrower(const Borrower& other) {
            this->limit = other.limit;
        }

        // copy assignment operator
        Borrower& operator=(const Borrower& other) {
            if (this != &other) {
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
                this->limit = std::min(limit, static_cast<unsigned int> (MAX_ITEMS_PER_BORROWER_HARD));
            } else {
                this->limit = limit;
            }
        }

        // void setBowId(unsigned long bow_id) {
        //     this->bow_id = bow_id;
        // }

        // getter method
        unsigned int getLimit() const {
            return this->limit;
        }

        // unsigned long getBowId() const {
        //     return this->bow_id;
        // }

        // serialization methods
        QString getSubclassType() const { return "Borrower"; }
        QJsonObject getSubclassParams() const;
        int loadSubclassParams(const QJsonObject& json);
        QJsonObject getSubclassSchema() const;
        Borrower(const QJsonObject& json) {
            if (loadSubclassParams(json) != 0) {
                throw std::runtime_error("Failed to load Borrower parameters");
            }
        }

};

#endif