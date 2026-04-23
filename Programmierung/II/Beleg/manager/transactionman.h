/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the TransactionMan class, which holds the transaction objects and provides methods to load and save the data from and to files.
Uses a hash map for fast access to transactions by libitem id and person id.
It also provides methods to add, remove and get transaction objects by id.
*/

#ifndef _TRANSACTIONMAN_H
#define _TRANSACTIONMAN_H

#include <iostream>
#include <QString>
#include <QVector>
#include <QHash>

#include "transaction.h"

class TransactionMan {
    QString filename;
    QVector<std::shared_ptr<Transaction>> transactions; // vector of transactions
    QHash<unsigned long, QVector<std::shared_ptr<Transaction>>> libitem_map; // hash map for fast access to all transactions containing a specific libitem id
    QHash<unsigned long, QVector<std::shared_ptr<Transaction>>> person_map; // hash map for fast access to all transactions containing a specific person id
    unsigned long next_id;

    // this method is private, because the next id is updated internally
    void setNextId(unsigned long id) {
        next_id = id;
    }

public:
    // constructor and destructor, loads the data from the file if load is true
    TransactionMan(const QString& filename, bool load = true);
    ~TransactionMan();

    // load and save the data from and to the file
    int load();
    int save();

    // Setters
    int setFilename(const QString& filename);

    // Getters
    QVector<std::shared_ptr<Transaction>> getTransactions() const;
    unsigned long getNextId() const;
    QString getFilename() const;

    // Management functions
    // add a new transaction to the vector and the hash maps
    int addTransaction(std::shared_ptr<Transaction> transaction);

    // remove a transaction by id
    int removeTransactionId(unsigned long id);
    // remove a transaction by index
    int removeTransaction(unsigned long index);

    // Retrieval functions
    // get all the transactions containing a specific libitem id
    QVector<std::shared_ptr<Transaction>> getTransactionsByLibitemId(unsigned long libitem_id) const;
    // get all the transactions containing a specific person id
    QVector<std::shared_ptr<Transaction>> getTransactionsByPersonId(unsigned long person_id) const;

    // get a transaction by index in the vector
    std::shared_ptr<Transaction> operator[](unsigned long idx);

    // print the transaction objects in the vector
    friend std::ostream& operator<<(std::ostream& os, const TransactionMan& tm);
};

#endif