/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the TransactionMan class, which holds the transaction objects and provides methods to load and save the data from and to files.
Uses a hash map for fast access to transactions by libitem id and person id.
It also provides methods to add, remove and get transaction objects by id.
*/

#include <iostream>
#include <QString>
#include <QVector>
#include <QHash>

#include "transaction.h"
#include "transactionman.h"

using namespace std;

// constructor, loads the data from the file if load is true
TransactionMan::TransactionMan(const QString& filename, bool load) {
    setFilename(filename);
    setNextId(0);
    if (load) {
        this->load();
    }
}

// destructor, saves the data to the file
TransactionMan::~TransactionMan() {
    save();
}

// load the data from the file
int TransactionMan::load() {
    QFile file(filename);

    // check if the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }

    // load each transaction from the file
    while (!file.atEnd()) {
        std::shared_ptr<Transaction> transaction = Transaction::fromFile(file);
        if (transaction) {
            // add the transaction to the vector and the hash maps
            addTransaction(transaction);
        } else {
            std::cerr << "Error loading transaction from file" << std::endl;
            break;
        }
    }
    return 0;
}

// save the data to the file
int TransactionMan::save() {
    QFile file(filename);

    // check if the file exists, if it is writeable and truncate it
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }

    // save each transaction to the file
    for (const auto& transaction : transactions) {
        transaction->toFile(file);
    }
    return 0;
}

// setter for the filename
int TransactionMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}

// add a new transaction to the vector and the hash maps
int TransactionMan::addTransaction(std::shared_ptr<Transaction> transaction) {
    if (transaction) {
        transactions.push_back(transaction);

        // update the next id if the new transaction id is greater than the current next id
        if (transaction->getId() >= next_id) {
            setNextId(transaction->getId() + 1);
        }

        // add the transaction to the hash maps
        libitem_map[transaction->getLibitemId()].push_back(transaction);
        person_map[transaction->getBorrowerId()].push_back(transaction);
        return 0;
    }
    std::cerr << "Error: Transaction is null" << std::endl;
    return -1;
}

// get all the transactions
QVector<std::shared_ptr<Transaction>> TransactionMan::getTransactions() const {
    return transactions;
}

// get the next id
unsigned long TransactionMan::getNextId() const {
    return next_id;
}

// get the filename
QString TransactionMan::getFilename() const {
    return filename;
}

// get all the transactions containing a specific person id
QVector<std::shared_ptr<Transaction>> TransactionMan::getTransactionsByPersonId(unsigned long person_id) const {
    // qDebug() << person_map.value(person_id).size() << "transactions for person ID:" << person_id;
    return person_map.value(person_id);
}

// remove a transaction by id
int TransactionMan::removeTransactionId(unsigned long id) {
    // iterate over the vector and remove the transaction if the id matches
    for (QVector<std::shared_ptr<Transaction>>::iterator it = transactions.begin(); it != transactions.end(); ++it) {
        if ((*it)->getId() == id) {
            transactions.erase(it);
            // remove the transaction from the hash maps
            libitem_map.remove((*it)->getLibitemId());
            person_map.remove((*it)->getBorrowerId());
            return 0;
        }
    }
    return -1;
}

// remove a transaction by index
int TransactionMan::removeTransaction(unsigned long index) {
    if (index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
        return 0;
    }
    std::cerr << "Error: Index out of range" << std::endl;
    return -1;
}

// get a transaction by index in the vector
std::shared_ptr<Transaction> TransactionMan::operator[](unsigned long idx) {
    // get transaction at index idx
    if (idx < (unsigned long) transactions.size()) {
        return transactions[idx];
    }
    std::cerr << "Error: Index out of range" << std::endl;
    return nullptr;
}

// print the transaction objects in the vector
std::ostream& operator<<(std::ostream& os, const TransactionMan& tm) {
    os << "TransactionMan: " << tm.getFilename().toStdString() << std::endl;
    os << "=========================" << std::endl;
    for (const auto& transaction : tm.getTransactions()) {
        os << *transaction << std::endl;
    }
    os << "========= End Transactionman =========" << std::endl;
    return os;
}
