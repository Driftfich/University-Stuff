#ifndef _TRANSACTION_H
#define _TRANSACTION_H

#include <QDateTime>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

#include "libitem.h"
#include "borrower.h"
#include "config.h"

class Transaction {
    unsigned long id;
    unsigned long libitem_id;
    unsigned long borrower_id; // Person id 
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
            setId(id);
            setLibitemId(libitem_id);
            setBorrowerId(borrower_id);
            setTransactionTime(transaction_time);
        }

        Transaction(QJsonObject json) {
            if (loadLocalParams(json) != 0) {
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

        // load json object from file
        int loadLocalParams(const QJsonObject& json);
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