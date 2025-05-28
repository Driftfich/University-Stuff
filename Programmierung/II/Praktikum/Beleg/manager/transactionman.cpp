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

using namespace std;

TransactionMan::TransactionMan(const QString& filename) {
    setFilename(filename);
    setNextId(0);
    load(); // internally the load function sets the next id with the last id + 1
    // debug the whole hash maps
    // std::cout << "Person Map Contents (" << person_map.size() << " entries):" << std::endl;
    // std::cout << "---------------------------------------" << std::endl;
    // if (person_map.isEmpty()) {
    //     std::cout << "  [Person Map is empty]" << std::endl;
    // } else {
    //     // Sort keys for consistent output order (optional)
    //     QList<unsigned long> person_keys = person_map.keys();
    //     std::sort(person_keys.begin(), person_keys.end());

    //     for (unsigned long person_id : person_keys) {
    //         const auto& transactions_for_person = person_map.value(person_id);
    //         std::cout << "Person ID: " << person_id << " (" << transactions_for_person.size() << " transaction(s))" << std::endl;
    //         if (transactions_for_person.isEmpty()) {
    //             std::cout << "  -> [No transactions associated]" << std::endl;
    //         } else {
    //             for (const auto& transaction_ptr : transactions_for_person) {
    //                 if (transaction_ptr) {
    //                     std::cout << "  -> " << *transaction_ptr << std::endl;
    //                 } else {
    //                     std::cout << "  -> [Null Transaction Pointer in person_map]" << std::endl;
    //                 }
    //             }
    //         }
    //     }
    // }
    // std::cout << "========= End Person Map =========" << std::endl;
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
        person_map[transaction->getBorrowerId()].push_back(transaction);
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

// QVector<std::shared_ptr<Transaction>> TransactionMan::getTransactionsByLibitemId(unsigned long libitem_id) const {
//     QVector<std::shared_ptr<Transaction>> transactions_by_libitem;
//     auto it = libitem_map.find(libitem_id);
//     if (it != libitem_map.end()) {
//         transactions_by_libitem = it.value();
//     }
//     return transactions_by_libitem;
// }
QVector<std::shared_ptr<Transaction>> TransactionMan::getTransactionsByPersonId(unsigned long person_id) const {
    // qDebug() << person_map.value(person_id).size() << "transactions for person ID:" << person_id;
    return person_map.value(person_id);
}

int TransactionMan::removeTransactionId(unsigned long id) {
    for (auto it = transactions.begin(); it != transactions.end(); ++it) {
        if ((*it)->getId() == id) {
            transactions.erase(it);
            libitem_map.remove((*it)->getLibitemId());
            person_map.remove((*it)->getBorrowerId());
            return 0;
        }
    }
    return -1;
}

int TransactionMan::removeTransaction(unsigned long index) {
    if (index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
        return 0;
    }
    std::cerr << "Error: Index out of range" << std::endl;
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
