#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include <QDate>
#include <QDateTime>
#include "transactiontablemodel.h"
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
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(transaction->getLibitemId());
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());
    std::shared_ptr<Person> person = personMan->getPerson(transaction->getBorrowerId());
    // if (transaction == nullptr || libitem == nullptr || media == nullptr || person == nullptr) {
    //     return QVariant();
    // }
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

void TransactionTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}