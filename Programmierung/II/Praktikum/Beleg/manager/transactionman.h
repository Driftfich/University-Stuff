// Transaction Manager class
// Manages all transaction objects
#ifndef _TRANSACTIONMAN_H
#define _TRANSACTIONMAN_H

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QHash>
#include <iostream>
#include "transaction.h"

class TransactionMan {
    QString filename;
    QVector<std::shared_ptr<Transaction>> transactions;
    QHash<unsigned long, QVector<std::shared_ptr<Transaction>>> libitem_map; // fast access to all transactions containing a specific libitem id
    QHash<unsigned long, QVector<std::shared_ptr<Transaction>>> person_map; // fast access to all transactions containing a specific person id
    unsigned long next_id;


    void setNextId(unsigned long id) {
        next_id = id;
    }

public:
    // constructor and destructor
    TransactionMan(const QString& filename);
    ~TransactionMan();

    // load and save functions
    int load();
    int save();

    // setters and getters
    int setFilename(const QString& filename);
    int addTransaction(std::shared_ptr<Transaction> transaction);
    
    QVector<std::shared_ptr<Transaction>> getTransactions() const;
    unsigned long getNextId() const;
    QString getFilename() const;

    // transaction management functions
    int removeTransactionId(unsigned long id);
    int removeTransaction(unsigned long index);

    std::shared_ptr<Transaction> operator[](unsigned long idx);

    // print functions
    friend std::ostream& operator<<(std::ostream& os, const TransactionMan& tm);


};

#endif