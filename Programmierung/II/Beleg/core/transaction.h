/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Transaction class, which holds information and logic related to transactions.
*/

#ifndef _TRANSACTION_H
#define _TRANSACTION_H

#include <QDateTime>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

#include "returns.h"
#include "libitem.h"

class Transaction {
    unsigned long id;
    unsigned long libitem_id;
    unsigned long borrower_id; // <=> Person ID
    QDateTime transaction_time;

    public:
        // setters
        int setId(unsigned long id) { this->id = id; return 0;}
        int setLibitemId(unsigned long libitem_id) { this->libitem_id = libitem_id; return 0;}
        int setBorrowerId(unsigned long borrower_id) { this->borrower_id = borrower_id; return 0;}
        int setTransactionTime(const QDateTime& transaction_time) { this->transaction_time = transaction_time; return 0;}

        // getters
        unsigned long getId() const { return id; }
        unsigned long getLibitemId() const { return libitem_id; }
        unsigned long getBorrowerId() const { return borrower_id; }
        QDateTime getTransactionTime() const { return transaction_time; }

        // constructor
        Transaction(unsigned long id, unsigned long libitem_id, unsigned long borrower_id, const QDateTime& transaction_time) {
            setId(id);
            setLibitemId(libitem_id);
            setBorrowerId(borrower_id);
            setTransactionTime(transaction_time);
        }

        Transaction(QJsonObject json) {
            Result result = loadLocalParams(json);
            // qDebug() << (int) result;
            // qDebug() << (QString) result;
            if (result != 0) {
                throw std::runtime_error("Issues loading transaction parameters");
            }
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

        // methods to load and save the transaction to/from a file
        QJsonObject getJson() const;
        void toFile(QFile& file) const;
        // load local parameters from a JSON object
        Result loadLocalParams(const QJsonObject& json);
        static std::shared_ptr<Transaction> fromFile(QFile& file);
        static std::shared_ptr<Transaction> TransactionFactory(const QJsonObject& json);
        
        // schema methods
        static QJsonObject getLocalSchema();
        static QJsonObject getSchema();

        // print method
        friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
            os << "Transaction ID: " << transaction.id << "\n"
               << "Libitem ID: " << transaction.libitem_id << "\n"
               << "Borrower ID: " << transaction.borrower_id << "\n"
               << "Transaction Timestamp: " << transaction.transaction_time.toString().toStdString() << "\n"
               << "----------------------------------------" << std::endl;
            return os;
        }
};

#endif