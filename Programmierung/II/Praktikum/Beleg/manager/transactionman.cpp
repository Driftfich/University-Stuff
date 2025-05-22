#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QHash>
#include <iostream>
#include "transactionman.h"
#include "transaction.h"
#include <iostream>

TransactionMan::TransactionMan(const QString& filename) {
    setFilename(filename);
    setNextId(0);
    load(); // internally the load function sets the next id with the last id + 1
}

TransactionMan::~TransactionMan() {
    save();
}

int TransactionMan::load() {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    while (!file.atEnd()) {
        std::shared_ptr<Transaction> transaction = Transaction::fromFile(file);
        if (transaction) {
            addTransaction(transaction);
        } else {
            std::cerr << "Error loading transaction from file" << std::endl;
            break;
        }
    }
    return 0;
}

int TransactionMan::save() {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
    for (const auto& transaction : transactions) {
        transaction->toFile(file);
    }
    return 0;
}

int TransactionMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}

int TransactionMan::addTransaction(std::shared_ptr<Transaction> transaction) {
    if (transaction) {
        transactions.push_back(transaction);
        if (transaction->getId() >= next_id) {
            setNextId(transaction->getId() + 1);
        }
        libitem_map[transaction->getLibitemId()].push_back(transaction);
        person_map[transaction->getId()].push_back(transaction);
        return 0;
    }
    std::cerr << "Error: Transaction is null" << std::endl;
    return -1;
}

QVector<std::shared_ptr<Transaction>> TransactionMan::getTransactions() const {
    return transactions;
}
unsigned long TransactionMan::getNextId() const {
    return next_id;
}
QString TransactionMan::getFilename() const {
    return filename;
}

int TransactionMan::removeTransaction(unsigned long id) {
    for (auto it = transactions.begin(); it != transactions.end(); ++it) {
        if ((*it)->getId() == id) {
            transactions.erase(it);
            libitem_map.remove((*it)->getLibitemId());
            person_map.remove((*it)->getId());
            return 0;
        }
    }
    return -1;
}

std::shared_ptr<Transaction> TransactionMan::operator[](unsigned long idx) {
    // get transaction at index idx
    if (idx < (unsigned long) transactions.size()) {
        return transactions[idx];
    }
    std::cerr << "Error: Index out of range" << std::endl;
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const TransactionMan& tm) {
    os << "TransactionMan: " << tm.getFilename().toStdString() << std::endl;
    os << "=========================" << std::endl;
    for (const auto& transaction : tm.getTransactions()) {
        os << *transaction << std::endl;
    }
    os << "========= End Transactionman =========" << std::endl;
    return os;
}
