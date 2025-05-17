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
#include "transaction.h"

class TransactionMan {
    QString filename;
    QVector<std::shared_ptr<Transaction>> transactions;
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
    int removeTransaction(unsigned long id);

    std::shared_ptr<Transaction> operator[](unsigned long idx);

    // print functions
    friend std::ostream& operator<<(std::ostream& os, const TransactionMan& tm);


};

#endif