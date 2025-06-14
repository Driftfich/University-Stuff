#include "persontablemodel.h"
#include "personman.h"
#include "transactionman.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <QDebug>
#include "person.h"
#include "personman.h"
#include "jsonschemautils.h"

#include "returns.h"


PersonTableModel::PersonTableModel(PersonMan* personMan, QObject *parent)
    : QAbstractTableModel(parent), personMan(personMan) {
    // Initialize the displayed columns with all columns
    for (int i = 0; i < MaxColumnIdentity; ++i) {
        displayedColumns.push_back(static_cast<ColumnIdentity>(i));
    }
}

PersonTableModel::~PersonTableModel() {
    // Destructor
}

int PersonTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return personMan->getPersons().size();
}
int PersonTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return displayedColumns.size();
}

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

QVariant PersonTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    unsigned long row = (unsigned long) index.row();
    int column = index.column();

    if (row >= (unsigned long) personMan->getPersons().size() || column < 0 || column >= displayedColumns.size()) {
        return QVariant();
    }
    std::shared_ptr<Person> person = (*personMan)[row];
    ColumnIdentity columnIdentity = displayedColumns[column];
    if (person == nullptr) {
        return QVariant();
    }
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

QVariant PersonTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        if (section < 0 || section >= displayedColumns.size()) {
            return QVariant();
        }
        ColumnIdentity columnIdentity = displayedColumns[section];
        QMap<ColumnIdentity, QString> columnNames = getAllColumnNames();
        return columnNames[columnIdentity];
    }
    return QVariant();
}

Qt::ItemFlags PersonTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// void PersonTableModel::setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns) {
//     beginResetModel();
//     this->displayedColumns = displayedColumns;
//     emit layoutChanged();
// }

void PersonTableModel::setDisplayedColumns(const QVector<QString>& displayedColumns) {
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
    emit layoutChanged();
    endResetModel();
}

QMap<PersonTableModel::ColumnIdentity, QString> PersonTableModel::getAllColumnNames() const {
    QMap<ColumnIdentity, QString> columnNames;
    columnNames[Id] = "ID";
    columnNames[FirstName] = "Vorname";
    columnNames[LastName] = "Nachname";
    columnNames[ExtensionName] = "Zusatzname";
    columnNames[BirthDate] = "Geburtsdatum";
    columnNames[Gender] = "Geschlecht";
    columnNames[Location] = "Standort";
    columnNames[Email] = "E-Mail";
    columnNames[Phone] = "Telefon";
    columnNames[Note] = "Notiz";
    return columnNames;
}

QVector<QString> PersonTableModel::getDisplayedColumns() const {
    QVector<QString> columnNames;
    for (const auto& column : displayedColumns) {
        columnNames.push_back(getAllColumnNames()[column]);
    }
    return columnNames;
}

QJsonObject PersonTableModel::getJsonObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) personMan->getPersons().size()) {
        return QJsonObject();
    }
    std::shared_ptr<Person> person = (*personMan)[row];
    if (!person) {
        return QJsonObject();
    }

    QJsonObject completeJson = QJsonObject();
    completeJson["person"] = person->getJson();
    return completeJson;
}

QJsonObject PersonTableModel::getJsonObject(const QModelIndex& index, const TransactionMan* transactionMan) const {
    QJsonObject completeJson = getJsonObject(index);
    if (completeJson.isEmpty()) {
        return QJsonObject();
    }
    QJsonObject personJson = completeJson["person"].toObject();
    // qDebug() << "Trying to fetch transactions for person ID:" << personJson["id"].toVariant().toULongLong();
    if (!transactionMan || !personJson.contains("id")) {
        return completeJson; // Return the person JSON if no transaction manager or id is not present
    }
    // debug the json
    // qDebug() << "Person JSON:" << QJsonDocument(personJson).toJson(QJsonDocument::Indented);
    // get all transactions for this person | get id from person from json
    unsigned long personId = personJson["id"].toVariant().toULongLong();
    QVector<std::shared_ptr<Transaction>> transactions = transactionMan->getTransactionsByPersonId(personId);
    QJsonArray transactionArray;
    for (const auto& transaction : transactions) {
        transactionArray.append(transaction->getJson());
    }
    // qDebug() << "Found" << transactionArray.size() << "transactions for person ID:" << personId;
    completeJson["transactions"] = transactionArray;
    return completeJson;
}

QJsonObject PersonTableModel::getDefaultJsonObject(bool artistChecked, bool borrowerChecked) const {
    // qDebug() << "Default Schema for Person Table Model: " << Person::getSchema() << "\n";
    QJsonObject defaultJson = createDefaultJsonFromSchema(Person::getSchema(artistChecked, borrowerChecked));
    // qDebug() << "Default JSON object for Person Table Model: " << defaultJson << "\n";
    defaultJson["id"] = QJsonValue::fromVariant(static_cast<quint64>(personMan->getNextId()));

    return QJsonObject{
        {"person", defaultJson}
    };
}

QJsonObject PersonTableModel::getDefaultSchema(bool ArtistChecked, bool BorrowerChecked) const {
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
                {"readonly", true},
                {"items", Transaction::getSchema()}
            }}
        }}
    };
}

QJsonObject PersonTableModel::getSchemaObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }
    
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) personMan->getPersons().size()) {
        return QJsonObject();
    }

    std::shared_ptr<Person> person = (*personMan)[row];
    if (!person) {
        return QJsonObject();
    }

    bool artistChecked = person->isArtist();
    bool borrowerChecked = person->isBorrower();
    
    return getDefaultSchema(artistChecked, borrowerChecked);
}

bool PersonTableModel::updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index) {
    if (!index.isValid()) {
        return false;
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) personMan->getPersons().size()) {
        return false;
    }
    std::shared_ptr<Person> person = (*personMan)[row];

    if (!person) {
        qWarning() << "Failed to get person from index";
        return false;
    }

    if (!jsonObject.contains("person")) {
        qWarning() << "JSON object does not contain 'person' key";
        return false;
    }

    if (person->loadLocalParams(jsonObject["person"].toObject()) != 0) { //  || person->loadSubclassParams(personJson["subclass_params"].toObject()) != 0
        qWarning() << "Failed to update person from JSON object";
        return false;
    }
    qDebug() << "Currently transactions isnt updated from JSON object";
    refreshData();
    return true;
}

Result PersonTableModel::saveFromJsonObject(const QJsonObject& jsonObject) {
    if (!jsonObject.contains("person")) {
        qWarning() << "JSON object does not contain 'person' key";
        return Result::Error("Failed to save person");
    }

    std::shared_ptr<Person> person = Person::PersonFactory(jsonObject["person"].toObject());
    if (!person) {
        qWarning() << "Failed to create person from JSON object";
        return Result::Error("Failed to create person");
    }
    std::cout << "Saving person: " << *person << std::endl;
    personMan->addPerson(person);
    refreshData();
    return Result::Success();
}

void PersonTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}
