#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include <QDate>
#include <QDateTime>
#include "transactiontablemodel.h"
#include "jsonschemautils.h"
#include "returns.h"
#include <transactionman.h>
#include <mediaman.h>
#include <libitemman.h>
#include <personman.h>


TransactionTableModel::TransactionTableModel(TransactionMan* transactionMan, PersonMan* personMan, LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent)
    : QAbstractTableModel(parent), transactionMan(transactionMan), personMan(personMan), libItemMan(libItemMan), mediaMan(mediaMan) {
    for (int i = 0; i < MaxColumnIdentity; ++i) {
        displayedColumns.push_back(static_cast<ColumnIdentity>(i));
    }
}

TransactionTableModel::~TransactionTableModel() {
    // Destructor
}

int TransactionTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return transactionMan->getTransactions().size();
}

int TransactionTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return displayedColumns.size();
}

int TransactionTableModel::removeRow(int row, const QModelIndex &parent) {
    // std::cout << "Trying to remove row: " << row << std::endl;
    if (row < 0 || row >= transactionMan->getTransactions().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row);
    // std::cout << "Removing row: " << row << std::endl;
    transactionMan->removeTransaction(row);
    endRemoveRows();
    return true;
}

bool TransactionTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row + count > transactionMan->getTransactions().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        transactionMan->removeTransaction(row);
    }
    endRemoveRows();
    return true;
}

QVariant TransactionTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    unsigned long row = (unsigned long) index.row();
    int column = index.column();

    if (row >= (unsigned long) transactionMan->getTransactions().size() || column < 0 || column >= displayedColumns.size()) {
        return QVariant();
    }
    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    // qDebug() << "Try to get the libitem\n";
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    // qDebug() << "Got the libitem\n";
    std::shared_ptr<Media> media;   
    if (libitem) media = mediaMan->getMedia(libitem->getMediaId());
    else media = nullptr;

    // qDebug() << "Got the media\n";
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    // if (transaction == nullptr || libitem == nullptr || media == nullptr || person == nullptr) {
    //     return QVariant();
    // }
    // qDebug() << "Loaded transaction, libitem, media, and person\n";
    ColumnIdentity columnIdentity = displayedColumns[column];
    switch (columnIdentity) {
        case Id:
            return static_cast<quint64>(transaction->getId());
        case LibitemId:
            return static_cast<quint64>(transaction->getLibitemId());
        case PersonId:
            return static_cast<quint64>(transaction->getBorrowerId());
        case Timestamp:
            return transaction->getTransactionTime().toString("yyyy-MM-dd hh:mm:ss");
        case MediaId:
            if (!libitem) return QVariant();
            return static_cast<quint64>(libitem->getMediaId());
        case Title:
            {
            if (!media) return QVariant();
            return media->getTitle();
            }
        case PublicationDate:
            if (!media) return QVariant();
            return media->getPublicationDate().toString("yyyy-MM-dd");
        case Publisher:
            if (!media) return QVariant();
            return media->getPublisher();
        case FirstName:
            if (!person) return QVariant();
            return person->getFname();
        case LastName:
            if (!person) return QVariant();
            return person->getLname();
        case ExtensionName:
            if (!person) return QVariant();
            return person->getEname();
        case BirthDate:
            if (!person) return QVariant();
            return person->getBirthday().toString("yyyy-MM-dd");
        default:
            return QVariant();
    }
}

QVariant TransactionTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        ColumnIdentity columnIdentity = displayedColumns[section];
        switch (columnIdentity) {
            case Id:
                return "ID";
            case LibitemId:
                return "Libitem ID";
            case PersonId:
                return "Person ID";
            case Timestamp:
                return "Timestamp";
            case MediaId:
                return "Media ID";
            case Title:
                return "Title";
            case PublicationDate:
                return "Publication Date";
            case Publisher:
                return "Publisher";
            case FirstName:
                return "First Name";
            case LastName:
                return "Last Name";
            case ExtensionName:
                return "Extension Name";
            case BirthDate:
                return "Birth Date";
            default:
                return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags TransactionTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractTableModel::flags(index);
}

// void TransactionTableModel::setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns) {
//     this->displayedColumns = displayedColumns;
//     emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
//     emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
// }

void TransactionTableModel::setDisplayedColumns(const QVector<QString>& displayedColumns) {
    beginResetModel();
    this->displayedColumns.clear();
    for (const auto& column : displayedColumns) {
        for (int i = 0; i < MaxColumnIdentity; ++i) {
            if (getAllColumnNames()[static_cast<ColumnIdentity>(i)] == column) {
                this->displayedColumns.push_back(static_cast<ColumnIdentity>(i));
                break;
            }
        }
    }
    endResetModel();
}

QMap<TransactionTableModel::ColumnIdentity, QString> TransactionTableModel::getAllColumnNames() const {
    QMap<ColumnIdentity, QString> columnNames;
    columnNames[Id] = "ID";
    columnNames[LibitemId] = "Libitem ID";
    columnNames[PersonId] = "Person ID";
    columnNames[Timestamp] = "Timestamp";
    columnNames[MediaId] = "Media ID";
    columnNames[Title] = "Title";
    columnNames[PublicationDate] = "Publication Date";
    columnNames[Publisher] = "Publisher";
    columnNames[FirstName] = "First Name";
    columnNames[LastName] = "Last Name";
    columnNames[ExtensionName] = "Extension Name";
    columnNames[BirthDate] = "Birth Date";
    return columnNames;
}

QVector<QString> TransactionTableModel::getDisplayedColumns() const {
    // substitute the displayedColumns with the actual column names
    QVector<QString> columnNames;
    for (const auto& column : displayedColumns) {
        columnNames.push_back(getAllColumnNames()[column]);
    }
    return columnNames;
}

QJsonObject TransactionTableModel::getJsonObject(const QModelIndex& index) const {
    QJsonObject jsonObject = QJsonObject();
    if (!index.isValid()) {
        return jsonObject;
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) transactionMan->getTransactions().size()) {
        return jsonObject;
    }
    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Media> media;
    if (!libitem) media = nullptr;
    else media = mediaMan->getMedia(libitem->getMediaId());
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());

    if (!transaction) {
        return jsonObject;
    }

    // add the json data to the main json object
    jsonObject["Transaction"] = transaction->getJson();
    if (libitem) {
        jsonObject["Libitem"] = libitem->getJson();
    } else {
        jsonObject["Libitem"] = QJsonObject();
    }
    if (media) {
        jsonObject["Media"] = media->getJson();
    } else {
        jsonObject["Media"] = QJsonObject();
    }
    if (person) {
        jsonObject["Person"] = person->getJson();
    } else {
        jsonObject["Person"] = QJsonObject();
    }

    return jsonObject;
}

QJsonObject TransactionTableModel::getDefaultJsonObject() const {
    QJsonObject defaultJsonObject = createDefaultJsonFromSchema(getDefaultSchema());
    defaultJsonObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(transactionMan->getNextId()));
    return defaultJsonObject;
}

QJsonObject TransactionTableModel::getSchemaObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) transactionMan->getTransactions().size()) {
        return QJsonObject();
    }

    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    if (!transaction) {
        return QJsonObject();
    }
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Media> media;
    if (!libitem) media = nullptr;
    else media = mediaMan->getMedia(libitem->getMediaId());

    QJsonObject rootSchema;
    rootSchema.insert("type", "object");
    QJsonObject properties;
    properties.insert("type", "object");
    properties.insert("Transaction", transaction->getSchema());
    if (libitem) {
        properties.insert("Libitem", libitem->getSchema());
    } else {
        properties.insert("Libitem", QJsonObject());
    }
    if (media) {
        // get the media type
        properties.insert("Media", media->getSchemaByType());
    } else {
        properties.insert("Media", QJsonObject());
    }
    if (person) {
        properties.insert("Person", person->getSchema());
    } else {
        properties.insert("Person", QJsonObject());
    }
    rootSchema.insert("properties", properties);
    return rootSchema;
}

QJsonObject TransactionTableModel::getDefaultSchema() const {
    QJsonObject defaultSchema = Transaction::getSchema();
    
    // Get the properties object
    QJsonObject properties = defaultSchema["properties"].toObject();
    // qDebug() << "Properties of Transaction Schema: " << properties << "\n";
    // Modify libitem_id field to make it not required
    if (properties.contains("libitem_id")) {
        QJsonObject libitemField = properties["libitem_id"].toObject();
        libitemField["readonly"] = false;
        properties["libitem_id"] = libitemField;
    }
    
    // Modify borrower_id field to make it not required
    if (properties.contains("borrower_id")) {
        QJsonObject borrowerField = properties["borrower_id"].toObject();
        borrowerField["readonly"] = false;
        properties["borrower_id"] = borrowerField;
    }
    // qDebug() << "Modified Properties of Transaction Schema: " << properties << "\n";
    // Put the modified properties back
    defaultSchema["properties"] = properties;
    return defaultSchema;
}

Result TransactionTableModel::saveFromJsonObject(const QJsonObject& jsonObject) {
    std::shared_ptr<Transaction> transaction = Transaction::TransactionFactory(jsonObject);

    // extract the person from the json object
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    if (!person) {
        return Result::Error("Person does not exist in the database");
    }

    // check that the libitem exists
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    if (!libitem) {
        return Result::Error("Libitem does not exist in the database");
    }

    // check that the person has not already reached the maximum number of borrowed items
    if (person->isBorrower()) {
        unsigned long limit = person->getBorrower()->getLimit();
        // count the number of current transactions for this person by utilizing the person map in the transaction manager
        QVector<std::shared_ptr<Transaction>> transactions = transactionMan->getTransactionsByPersonId(person->getId());
        if (transactions.size() >= limit) {
            QString errorMsg = person->getFname() + " " + person->getLname() + " has reached the maximum number of borrowed items (" + QString::number(limit) + ")";
            return Result::Error(errorMsg);
        }
    }
    else {
        return Result::Error("Person is not a borrower");
    }

    if (!transaction) {
        return Result::Error("Failed to create transaction from JSON");
    }

    // Add the new transaction
    transactionMan->addTransaction(transaction);
    refreshData();
    return Result::Success();
}

bool TransactionTableModel::updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index) {
    if (!index.isValid()) {
        return false;
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) transactionMan->getTransactions().size()) {
        return false;
    }
    // qDebug() << "Try to update transaction from json object" << "\n";
    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    std::shared_ptr<Media> media = nullptr;

    if (!transaction) {
        return false;
    }
    if (libitem) {
        media = mediaMan->getMedia(libitem->getMediaId());
    }

    // Update the transaction from the json object
    if (transaction->loadLocalParams(jsonObject["Transaction"].toObject()) != 0) {
        return false;
    }

    // Update the libitem, media and person if they exist in the json object
    if (libitem && jsonObject.contains("Libitem")) {
        if (libitem->loadLocalParams(jsonObject["Libitem"].toObject()) != 0) {
            return false;
        }
    }
    
    if (media && jsonObject.contains("Media")) {
        QJsonObject mediaObject = jsonObject["Media"].toObject();
        if (media->loadLocalParams(mediaObject) != 0) {
            return false;
        }
        if (mediaObject.contains("subclass_parameters")) {
            QJsonObject subclassParams = mediaObject["subclass_parameters"].toObject();
            if (media->loadSubclassParams(subclassParams) != 0) {
                return false;
            }
        }
    }

    if (person && jsonObject.contains("Person")) {
        QJsonObject personObject = jsonObject["Person"].toObject();
        if (person->loadLocalParams(personObject) != 0) {
            return false;
        }
    }

    refreshData();
    return true;
}

void TransactionTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}