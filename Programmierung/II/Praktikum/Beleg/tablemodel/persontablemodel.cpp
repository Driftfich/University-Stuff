/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
Custom Qt table model to display the personman class in the mainwindow.
*/


#include <iostream>

// include Qt classes
#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

// include data classes
#include "person.h"
#include "personman.h"
#include "transactionman.h"
#include "persontablemodel.h"

// include utils
#include "jsonschemautils.h"
#include "returns.h"

// constructor
PersonTableModel::PersonTableModel(PersonMan* personMan, QObject *parent)
    : QAbstractTableModel(parent), personMan(personMan) {
    // Initialize the displayed columns with all columns
    for (int i = 0; i < MaxColumnIdentity; ++i) {
        displayedColumns.push_back(static_cast<ColumnIdentity>(i));
    }
}

// destructor
PersonTableModel::~PersonTableModel() {
}

// override the rowCount method to return the number of rows
int PersonTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return personMan->getPersons().size();
}

// override the columnCount method to return the number of columns in the displayedColumns vector
int PersonTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return displayedColumns.size();
}

// remove a row from the person manager
int PersonTableModel::removeRow(int row, const QModelIndex &parent) {
    // std::cout << "Trying to remove row: " << row << std::endl;
    if (row < 0 || row >= personMan->getPersons().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row);
    // std::cout << "Removing row: " << row << std::endl;
    personMan->removePerson(row);
    endRemoveRows();
    return true;
}

// override the removeRows method to remove multiple rows from the person manager
bool PersonTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row + count > personMan->getPersons().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        personMan->removePerson(row);
    }
    endRemoveRows();
    return true;
}

// override the data method to return the data for a specific index and role
QVariant PersonTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    
    // get the row and column from the index
    unsigned long row = (unsigned long) index.row();
    int column = index.column();

    // check row and column bounds
    if (row >= (unsigned long) personMan->getPersons().size() || column < 0 || column >= displayedColumns.size()) {
        return QVariant();
    }

    // get the person from the row
    std::shared_ptr<Person> person = (*personMan)[row];
    // get the column identity from the displayedColumns vector
    ColumnIdentity columnIdentity = displayedColumns[column];
    // check if the person is valid
    if (person == nullptr) {
        return QVariant();
    }

    // return the data for the specific column
    switch (columnIdentity) {
        case Id:
            return static_cast<quint64>(person->getId());
        case FirstName:
            return person->getFname();
        case LastName:
            return person->getLname();
        case ExtensionName:
            return person->getEname();
        case BirthDate:
            return person->getBirthday().toString("yyyy-MM-dd");
        case Gender:
            return person->getGenderString();
        case Location:
            return person->getLocation();
        case Email:
            return person->getEmail();
        case Phone:
            return person->getTel();
        case Note:
            return person->getNote();
        default:
            return QVariant();
    }
}

// override the headerData method to return the header data for a specific section and orientation
QVariant PersonTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    // check if the orientation is horizontal
    if (orientation == Qt::Horizontal) {
        if (section < 0 || section >= displayedColumns.size()) {
            return QVariant();
        }
        // get the column identity from the displayedColumns vector
        ColumnIdentity columnIdentity = displayedColumns[section];
        // get the column names from the getAllColumnNames method
        QMap<ColumnIdentity, QString> columnNames = getAllColumnNames();
        // return the column name
        return columnNames[columnIdentity];
    }
    return QVariant();
}

// override the flags method to return the flags for a specific index
Qt::ItemFlags PersonTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// update the displayed columns
void PersonTableModel::setDisplayedColumns(const QVector<QString>& displayedColumns) {
    beginResetModel();
    // clear the displayedColumns vector
    this->displayedColumns.clear();
    // add the new displayed columns to the displayedColumns vector
    for (const auto& column : displayedColumns) {
        for (int i = 0; i < MaxColumnIdentity; ++i) {
            if (getAllColumnNames()[static_cast<ColumnIdentity>(i)] == column) {
                this->displayedColumns.push_back(static_cast<ColumnIdentity>(i));
                break;
            }
        }
    }
    // emit the layoutChanged signal to update the view
    emit layoutChanged();
    endResetModel();
}

// get all the available column names
QMap<PersonTableModel::ColumnIdentity, QString> PersonTableModel::getAllColumnNames() const {
    QMap<ColumnIdentity, QString> columnNames;
    columnNames[Id] = "ID";
    columnNames[FirstName] = "First Name";
    columnNames[LastName] = "Last Name";
    columnNames[ExtensionName] = "Extension Name";
    columnNames[BirthDate] = "Birth Date";
    columnNames[Gender] = "Gender";
    columnNames[Location] = "Location";
    columnNames[Email] = "Email";
    columnNames[Phone] = "Phone";
    columnNames[Note] = "Note";
    return columnNames;
}

// get the displayed columns
QVector<QString> PersonTableModel::getDisplayedColumns() const {
    QVector<QString> columnNames;
    for (const auto& column : displayedColumns) {
        columnNames.push_back(getAllColumnNames()[column]);
    }
    return columnNames;
}

// get the json object for a specific person at a specific index to show in infopanel
QJsonObject PersonTableModel::getJsonObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    // get the row from the index
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) personMan->getPersons().size()) {
        return QJsonObject();
    }

    // get the person from the row
    std::shared_ptr<Person> person = (*personMan)[row];
    if (!person) {
        return QJsonObject();
    }

    // create the json object with the person
    QJsonObject completeJson = QJsonObject();
    completeJson["person"] = person->getJson();
    return completeJson;
}

// get the json object for a specific person at a specific index to show in infopanel with transactions for the person
QJsonObject PersonTableModel::getJsonObject(const QModelIndex& index, const TransactionMan* transactionMan) const {
    // get the json object for the person
    QJsonObject completeJson = getJsonObject(index);
    if (completeJson.isEmpty()) {
        return QJsonObject();
    }

    // get the person json object
    QJsonObject personJson = completeJson["person"].toObject();
    // check if the transaction manager is valid and the person id is present
    if (!transactionMan || !personJson.contains("id")) {
        return completeJson; // Return the person JSON if no transaction manager or id is not present
    }

    // get the person id from the person json object
    unsigned long personId = personJson["id"].toVariant().toULongLong();
    // get all transactions for this person
    QVector<std::shared_ptr<Transaction>> transactions = transactionMan->getTransactionsByPersonId(personId);
    // create the transaction array
    QJsonArray transactionArray;
    for (const auto& transaction : transactions) {
        transactionArray.append(transaction->getJson());
    }
    // add the transaction array to the complete json object
    completeJson["transactions"] = transactionArray;
    return completeJson;
}

// get the default json object with artist and borrower attributes if checked
QJsonObject PersonTableModel::getDefaultJsonObject(bool artistChecked, bool borrowerChecked) const {
    // create the default json object from the schema
    QJsonObject defaultJson = createDefaultJsonFromSchema(Person::getSchema(artistChecked, borrowerChecked));
    // set the id to the next id
    defaultJson["id"] = QJsonValue::fromVariant(static_cast<quint64>(personMan->getNextId()));

    // return the default json object
    return QJsonObject{
        {"person", defaultJson}
    };
}

// get the default schema with artist and borrower attributes if checked
QJsonObject PersonTableModel::getDefaultSchema(bool ArtistChecked, bool BorrowerChecked) const {
    // create the transaction schema
    QJsonObject transaction = Transaction::getSchema();
    // make the transaction object readonly
    transaction["readonly"] = true;
    return QJsonObject{
        {"type", "object"},
        {"properties", QJsonObject{
            {"person", QJsonObject{
                {"type", "object"},
                {"rename", "Person"},
                {"description", "Person object containing personal information"},
                {"properties", Person::getSchema(ArtistChecked, BorrowerChecked).value("properties").toObject()}
            }},
            {"transactions", QJsonObject{
                {"type", "array"},
                {"rename", "Transactions"},
                {"description", "List of transactions for this person"},
                {"readonly", true}, // make the transactions readonly
                {"items", Transaction::getSchema()}
            }}
        }}
    };
}

// get the schema object for a specific person at a specific index to show in infopanel
QJsonObject PersonTableModel::getSchemaObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    // get the row from the index
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) personMan->getPersons().size()) {
        return QJsonObject();
    }

    // get the person from the row
    std::shared_ptr<Person> person = (*personMan)[row];
    if (!person) {
        return QJsonObject();
    }

    // get the artist and borrower attributes from the person
    bool artistChecked = person->isArtist();
    bool borrowerChecked = person->isBorrower();

    // return the default schema with artist and borrower attributes if checked
    return getDefaultSchema(artistChecked, borrowerChecked);
}

// update the person at specific index from a json object
bool PersonTableModel::updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index) {
    if (!index.isValid()) {
        return false;
    }

    // get the row from the index
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) personMan->getPersons().size()) {
        return false;
    }

    // get the person from the row
    std::shared_ptr<Person> person = (*personMan)[row];
    // check if the person is valid
    if (!person) {
        qWarning() << "Failed to get person from index";
        return false;
    }

    // check if the json object contains the person key
    if (!jsonObject.contains("person")) {
        qWarning() << "JSON object does not contain 'person' key";
        return false;
    }

    // update the person from the json object
    if (person->loadLocalParams(jsonObject["person"].toObject()) != 0) { //  || person->loadSubclassParams(personJson["subclass_params"].toObject()) != 0
        qWarning() << "Failed to update person from JSON object";
        return false;
    }
    // refresh the data
    refreshData();
    return true;
}

// save a new person from a json object
Result PersonTableModel::saveFromJsonObject(const QJsonObject& jsonObject) {
    if (!jsonObject.contains("person")) {
        qWarning() << "JSON object does not contain 'person' key";
        return Result::Error("Failed to save person");
    }

    // create a new person from the json object
    std::shared_ptr<Person> person = Person::PersonFactory(jsonObject["person"].toObject());
    // check if the person is valid
    if (!person) {
        qWarning() << "Failed to create person from JSON object";
        return Result::Error("Failed to create person");
    }

    // add the person to the person manager
    personMan->addPerson(person);
    // refresh the data
    refreshData();
    return Result::Success();
}

// refresh the data in the table model
void PersonTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}
