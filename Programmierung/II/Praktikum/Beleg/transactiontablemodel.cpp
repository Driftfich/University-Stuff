#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
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
    if (transaction == nullptr || libitem == nullptr || media == nullptr || person == nullptr) {
        return QVariant();
    }
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
            return static_cast<quint64>(libitem->getMediaId());
        case Title:
            return media->getTitle();
        case PublicationDate:
            return media->getPublicationDate().toString("yyyy-MM-dd");
        case Publisher:
            return media->getPublisher();
        case FirstName:
            return person->getFirstName();
        case LastName:
            return person->getLastName();
        case ExtensionName:
            return person->getExtensionName();
        case BirthDate:
            return person->getBirthDate().toString("yyyy-MM-dd");
        default:
            return QVariant();
    }
}

Qt::ItemFlags TransactionTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractTableModel::flags(index);
}

void TransactionTableModel::setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns) {
    this->displayedColumns = displayedColumns;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
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

QVector<TransactionTableModel::ColumnIdentity> TransactionTableModel::getDisplayedColumns() const {
    return displayedColumns;
}

void TransactionTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}