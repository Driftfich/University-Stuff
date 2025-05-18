
#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>

#include "libitemtablemodel.h"
#include "libitemman.h"
#include "libitem.h"
#include "media.h"

LibItemTableModel::LibItemTableModel(LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent)
    : QAbstractTableModel(parent), libItemMan(libItemMan), mediaMan(mediaMan) {
    for (int i = 0; i < MaxColumnIdentity; ++i) {
        displayedColumns.push_back(static_cast<ColumnIdentity>(i));
    }
}
LibItemTableModel::~LibItemTableModel() {
    // Destructor
}
int LibItemTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return libItemMan->getLibitems().size();
}
int LibItemTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return displayedColumns.size();
}

QVariant LibItemTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    unsigned long row = (unsigned long) index.row();
    int column = index.column();

    if (row >= (unsigned long) libItemMan->getLibitems().size() || column < 0 || column >= displayedColumns.size()) {
        return QVariant();
    }
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());
    if (libitem == nullptr || media == nullptr) {
        return QVariant();
    }
    ColumnIdentity columnIdentity = displayedColumns[column];
    switch (columnIdentity) {
        case Id:
            return static_cast<quint64>(libitem->getId());
        case MediaId:
            return static_cast<quint64>(libitem->getMediaId());
        case AvailableCopies:
            return static_cast<quint64>(libitem->getAvailableCopies());
        case BorrowedCopies:
            return static_cast<quint64>(libitem->getBorrowedCopies());
        case Location:
            return libitem->getLocation();
        case Condition:
            return libitem->getCondition();
        case Title:
            return media->getTitle();
        case PublicationDate:
            return media->getPublicationDate().toString("yyyy-MM-dd");
        case Publisher:
            return media->getPublisher();
        case Description:
            return media->getDescription();
        case Genre:
            return media->getGenre();
        case Languages:
            return media->getLanguages().join(", ");
        default:
            return QVariant();
    }
}

QVariant LibItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        ColumnIdentity columnIdentity = displayedColumns[section];
        switch (columnIdentity) {
            case Id:
                return "ID";
            case MediaId:
                return "Media ID";
            case AvailableCopies:
                return "Available Copies";
            case BorrowedCopies:
                return "Borrowed Copies";
            case Location:
                return "Location";
            case Condition:
                return "Condition";
            case Title:
                return "Title";
            case PublicationDate:
                return "Publication Date";
            case Publisher:
                return "Publisher";
            case Description:
                return "Description";
            case Genre:
                return "Genre";
            case Languages:
                return "Languages";
            default:
                return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags LibItemTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractTableModel::flags(index);
}

void LibItemTableModel::setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns) {
    this->displayedColumns = displayedColumns;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

QMap<LibItemTableModel::ColumnIdentity, QString> LibItemTableModel::getAllColumnNames() const {
    QMap<ColumnIdentity, QString> columnNames;
    columnNames[Id] = "ID";
    columnNames[MediaId] = "Media ID";
    columnNames[AvailableCopies] = "Available Copies";
    columnNames[BorrowedCopies] = "Borrowed Copies";
    columnNames[Location] = "Location";
    columnNames[Condition] = "Condition";
    columnNames[Title] = "Title";
    columnNames[PublicationDate] = "Publication Date";
    columnNames[Publisher] = "Publisher";
    columnNames[Description] = "Description";
    columnNames[Genre] = "Genre";
    columnNames[Languages] = "Languages";
    return columnNames;
}

QVector<LibItemTableModel::ColumnIdentity> LibItemTableModel::getDisplayedColumns() const {
    return displayedColumns;
}

void LibItemTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}