#ifndef _TRANSACTION_H
#define _TRANSACTION_H

#include <QDateTime>

#include "libitem.h"
#include "borrower.h"
#include "config.h"

class Transaction {
    unsigned long id;
    unsigned long libitem_id;
    unsigned long borrower_id;
    QDateTime transaction_time;

    public:
        // setters
        void setId(unsigned long id) { this->id = id; }
        void setLibitemId(unsigned long libitem_id) { this->libitem_id = libitem_id; }
        void setBorrowerId(unsigned long borrower_id) { this->borrower_id = borrower_id; }
        void setTransactionTime(const QDateTime& transaction_time) { this->transaction_time = transaction_time; }

        // getters
        unsigned long getId() const { return id; }
        unsigned long getLibitemId() const { return libitem_id; }
        unsigned long getBorrowerId() const { return borrower_id; }
        QDateTime getTransactionTime() const { return transaction_time; }

        // constructor
        Transaction(unsigned long id, unsigned long libitem_id, unsigned long borrower_id, const QDateTime& transaction_time) {
            this->id = id;
            this->libitem_id = libitem_id;
            this->borrower_id = borrower_id;
            this->transaction_time = transaction_time;
        }

        // destructor
        ~Transaction() {}

        // copy constructor
        Transaction(const Transaction& other) {
            this->id = other.id;
            this->libitem_id = other.libitem_id;
            this->borrower_id = other.borrower_id;
            this->transaction_time = other.transaction_time;
        }

        // assignment operator
        Transaction& operator=(const Transaction& other) {
            if (this != &other) {
                this->id = other.id;
                this->libitem_id = other.libitem_id;
                this->borrower_id = other.borrower_id;
                this->transaction_time = other.transaction_time;
            }
            return *this;
        }
};

#endif