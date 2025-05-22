#include "persontablemodel.h"
#include "personman.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include "person.h"
#include "personman.h"


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

void PersonTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}
