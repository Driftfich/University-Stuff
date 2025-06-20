/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
Custom Qt table model to display the transactionman class in the mainwindow.
*/

// include Qt classes
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

// constructor
TransactionTableModel::TransactionTableModel(TransactionMan* transactionMan, PersonMan* personMan, LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent)
    : QAbstractTableModel(parent), transactionMan(transactionMan), personMan(personMan), libItemMan(libItemMan), mediaMan(mediaMan) {
    for (int i = 0; i < MaxColumnIdentity; ++i) {
        displayedColumns.push_back(static_cast<ColumnIdentity>(i));
    }
}

// destructor
TransactionTableModel::~TransactionTableModel() {
}

// override the rowCount method to return the number of rows
int TransactionTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return transactionMan->getTransactions().size();
}

// override the columnCount method to return the number of columns in the displayedColumns vector
int TransactionTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return displayedColumns.size();
}

// remove a row from the transaction manager
int TransactionTableModel::removeRow(int row, const QModelIndex &parent) {
    // std::cout << "Trying to remove row: " << row << std::endl;
    if (row < 0 || row >= transactionMan->getTransactions().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row);
    // std::cout << "Removing row: " << row << std::endl;
    // remove the transaction from the transaction manager
    transactionMan->removeTransaction(row);
    endRemoveRows();
    return true;
}

// override the removeRows method to remove multiple rows from the transaction manager
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

// override the data method to return the data for a specific index and role
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

// override the headerData method to return the header data for a specific section and orientation
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

// override the flags method to return the flags for a specific index
Qt::ItemFlags TransactionTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractTableModel::flags(index);
}

// update the displayed columns
void TransactionTableModel::setDisplayedColumns(const QVector<QString>& displayedColumns) {
    beginResetModel();
    // clear the displayedColumns vector
    this->displayedColumns.clear();
    // add the new displayed columns to the displayedColumns vector
    for (const auto& column : displayedColumns) {
        for (int i = 0; i < MaxColumnIdentity; ++i) {
            // check if the column name is in the displayedColumns vector
            if (getAllColumnNames()[static_cast<ColumnIdentity>(i)] == column) {
                this->displayedColumns.push_back(static_cast<ColumnIdentity>(i));
                break;
            }
        }
    }
    endResetModel();
}

// get all the available column names
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

// get the displayed columns
QVector<QString> TransactionTableModel::getDisplayedColumns() const {
    // substitute the displayedColumns with the actual column names
    QVector<QString> columnNames;
    for (const auto& column : displayedColumns) {
        columnNames.push_back(getAllColumnNames()[column]);
    }
    return columnNames;
}

// get the json object for a specific transaction at a specific index to show in infopanel
QJsonObject TransactionTableModel::getJsonObject(const QModelIndex& index) const {
    QJsonObject jsonObject = QJsonObject();
    if (!index.isValid()) {
        return jsonObject;
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) transactionMan->getTransactions().size()) {
        return jsonObject;
    }
    // get the transaction, libitem, media and person from the row
    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Media> media;
    if (!libitem) media = nullptr;
    else media = mediaMan->getMedia(libitem->getMediaId());
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());

    // check if the transaction is valid
    if (!transaction) {
        return jsonObject;
    }

    // add the json data to the main json object
    jsonObject["Transaction"] = transaction->getJson();
    if (libitem) {
        QJsonObject libitemJson = libitem->getJson();
        // overwrite the borrowed copies in the libitem json object with the amount of transactions containing the libitem id
        libitemJson["borrowed_copies"] = static_cast<qint64>(transactionMan->getTransactionsByLibitemId(libitem->getId()).size());
        jsonObject["Libitem"] = libitemJson;
    } else {
        jsonObject["Libitem"] = QJsonObject();
    }
    if (media) {
        jsonObject["Media"] = media->getJson();
    } else {
        jsonObject["Media"] = QJsonObject();
    }
    if (person) {
        jsonObject["person"] = person->getJson();
    } else {
        jsonObject["person"] = QJsonObject();
    }

    return jsonObject;
}

// get the default json object for a new transaction
QJsonObject TransactionTableModel::getDefaultJsonObject() const {
    QJsonObject defaultJsonObject = createDefaultJsonFromSchema(getDefaultSchema());
    defaultJsonObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(transactionMan->getNextId()));
    return defaultJsonObject;
}

// get the schema object for a specific transaction at a specific index to show in infopanel
QJsonObject TransactionTableModel::getSchemaObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) transactionMan->getTransactions().size()) {
        return QJsonObject();
    }
    
    // get the transaction, person, libitem and media for the row
    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    if (!transaction) {
        return QJsonObject();
    }
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Media> media;
    if (!libitem) media = nullptr;
    else media = mediaMan->getMedia(libitem->getMediaId());

    // create the root schema and add the transaction, libitem, media and person schemas
    QJsonObject rootSchema;
    rootSchema.insert("type", "object");
    QJsonObject properties;
    properties.insert("type", "object");
    properties.insert("Transaction", transaction->getSchema());
    if (libitem) {
        QJsonObject libitemSchema = libitem->getSchema();
        // make the media id readonly so the media reference cannot be changed in the transaction infopanel (do this in the libitem infopanel)
        QJsonObject libitemProperties = libitemSchema["properties"].toObject();
        QJsonObject mediaId = libitemProperties["media_id"].toObject();
        mediaId["readonly"] = true;
        libitemProperties["media_id"] = mediaId;
        libitemSchema["properties"] = libitemProperties;
        properties.insert("Libitem", libitemSchema);
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
        properties.insert("person", person->getSchemaAuto());
    } else {
        properties.insert("person", QJsonObject());
    }
    rootSchema.insert("properties", properties);
    return rootSchema;
}

// get the default schema for a new transaction
QJsonObject TransactionTableModel::getDefaultSchema() const {
    QJsonObject defaultSchema = Transaction::getSchema();
    
    // Get the properties object
    QJsonObject properties = defaultSchema["properties"].toObject();
    // qDebug() << "Properties of Transaction Schema: " << properties << "\n";
    // Modify libitem_id field to make it not required
    if (properties.contains("libitem_id")) {
        QJsonObject libitemField = properties["libitem_id"].toObject();
        libitemField["readonly"] = false; // make the libitem id editable in the form
        properties["libitem_id"] = libitemField;
    }
    
    // Modify borrower_id field to make it not required
    if (properties.contains("borrower_id")) {
        QJsonObject borrowerField = properties["borrower_id"].toObject();
        borrowerField["readonly"] = false; // make the borrower id editable in the form
        properties["borrower_id"] = borrowerField;
    }
    // qDebug() << "Modified Properties of Transaction Schema: " << properties << "\n";
    // Put the modified properties back
    defaultSchema["properties"] = properties;
    return defaultSchema;
}

// save a new transaction from a json object
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

    // check how often the libitem is already borrowed
    QVector<std::shared_ptr<Transaction>> transactions = transactionMan->getTransactionsByLibitemId(libitem->getId());
    if (transactions.size() >= libitem->getAvailableCopies()) {
        QString errorMsg = "Libitem " + QString::number(libitem->getId()) + " has reached the maximum number of available copies (" + QString::number(libitem->getAvailableCopies()) + ")";
        return Result::Error(errorMsg);
    }

    // check if the person is a borrower
    if (person->isBorrower()) {
        // get the limit of the person
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

// update a transaction at a specific index from a json object
bool TransactionTableModel::updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index) {
    qDebug() << "Updating transaction from json object: " << jsonObject << "\n";
    if (!index.isValid()) {
        return false;
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) transactionMan->getTransactions().size()) {
        return false;
    }

    // get the transaction, libitem, media and person from the row
    std::shared_ptr<Transaction> transaction = (*transactionMan)[row];
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    std::shared_ptr<Media> media = nullptr;

    if (!transaction) {
        return false;
    }
    if (libitem) {
        // get the media from the libitem
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
    
    // update the person if it exists in the json object
    if (person && jsonObject.contains("person")) {
        QJsonObject personObject = jsonObject["person"].toObject();
        if (person->loadLocalParams(personObject) != 0) {
            return false;
        }
    }
    
    // update the media if it exists in the json object
    if (media && jsonObject.contains("Media")) {
        QJsonObject mediaObject = jsonObject["Media"].toObject();
        if (mediaObject.contains("media")) {
            QJsonObject baseMedia = mediaObject["media"].toObject();
            if (media->loadLocalParams(baseMedia) != 0) {
                return false;
            }
        }
        if (mediaObject.contains("subclass_parameters")) {
            QJsonObject subclassParams = mediaObject["subclass_parameters"].toObject();
            if (!subclassParams.isEmpty() && media->loadSubclassParams(subclassParams) != 0) {
                return false;
            }
        }
    }

    refreshData();
    return true;
}

void TransactionTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}