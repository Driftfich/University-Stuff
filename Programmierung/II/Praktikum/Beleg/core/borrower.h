/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Borrower class, which holds information and logic related to borrowers.
*/

#ifndef _BORROWER_H
#define _BORROWER_H

#include "config.h"
#include "returns.h"

using namespace std;

class Borrower {
    unsigned int limit; // Limit the amount of items one borrower can hold at the same time

    public:
        // constructor
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
        int setLimit(unsigned int limit) {
            // Limit the number of items a borrower can hold
            if (MAX_ITEMS_PER_BORROWER_HARD != -1) {
                this->limit = std::min(limit, static_cast<unsigned int> (MAX_ITEMS_PER_BORROWER_HARD));
            } else {
                this->limit = limit;
            }
            return 0;
        }

        // getter method
        unsigned int getLimit() const {
            return this->limit;
        }

        // serialization methods
        QString getSubclassType() const { return "Borrower"; }
        QJsonObject getSubclassParams() const;
        Result loadSubclassParams(const QJsonObject& json);
        static QJsonObject getSubclassSchema(bool checked = false);

        // constructor for loading from JSON
        Borrower(const QJsonObject& json) {
            if (loadSubclassParams(json) != 0) {
                throw std::runtime_error("Failed to load Borrower parameters");
            }
        }

};

#endif