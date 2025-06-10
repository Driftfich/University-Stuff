
#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>

#include "jsonschemautils.h"
#include "libitemtablemodel.h"
#include "libitemman.h"
#include "libitem.h"
#include "media.h"
#include "video.h"
#include "text.h"
#include "audio.h"

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

int LibItemTableModel::removeRow(int row, const QModelIndex &parent) {
    // std::cout << "Trying to remove row: " << row << std::endl;
    if (row < 0 || row >= libItemMan->getLibitems().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row);
    // std::cout << "Removing row: " << row << std::endl;
    libItemMan->removeLibitem(row);
    endRemoveRows();
    return true;
}

bool LibItemTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row + count > libItemMan->getLibitems().size()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        libItemMan->removeLibitem(row);
    }
    endRemoveRows();
    return true;
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
    if (libitem == nullptr) {
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
            if (!media) return QVariant();
            return media->getTitle();
        case PublicationDate:
            if (!media) return QVariant();
            return media->getPublicationDate().toString("yyyy-MM-dd");
        case Publisher:
            if (!media) return QVariant();
            return media->getPublisher();
        case Description:
            if (!media) return QVariant();
            return media->getDescription();
        case Genre:
            if (!media) return QVariant();
            return media->getGenre();
        case Languages:
            if (!media) return QVariant();
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

// void LibItemTableModel::setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns) {
//     this->displayedColumns = displayedColumns;
//     emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
//     emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
// }

void LibItemTableModel::setDisplayedColumns(const QVector<QString>& displayedColumns) {
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

QVector<QString> LibItemTableModel::getDisplayedColumns() const {
    QVector<QString> columnNames;
    for (const auto& column : displayedColumns) {
        columnNames.push_back(getAllColumnNames()[column]);
    }
    return columnNames;
}

QJsonObject LibItemTableModel::getJsonObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) libItemMan->getLibitems().size()) {
        return QJsonObject();
    }
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    if (!libitem) {
        return QJsonObject();
    }
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    QJsonObject jsonObject = QJsonObject();
    jsonObject["libitem"] = (*libitem).getJson();
    if (media) {
        QJsonObject mediaJson = (*media).getJson();
        if (mediaJson.contains("subclass_params") && mediaJson["subclass_params"].toObject().isEmpty()) {
            mediaJson.remove("subclass_params");
        }
        jsonObject["media"] = mediaJson;
    }
    else {
        jsonObject["media"] = QJsonObject();
    }
    return jsonObject;
}

QJsonObject LibItemTableModel::getSchemaObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) libItemMan->getLibitems().size()) {
        return QJsonObject();
    }
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    if (!libitem) {
        return QJsonObject();
    }
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    QJsonObject rootSchema;
    rootSchema.insert("type", "object");
    QJsonObject properties;
    properties.insert("type", "object");
    properties.insert("libitem", libitem->getSchema());
    if (media) {
        properties.insert("media", media->getSchema());
    } else {
        properties.insert("media", QJsonObject());
    }
    rootSchema.insert("properties", properties);
    return rootSchema;
}

bool LibItemTableModel::updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index) {
    if (!index.isValid()) {
        return false;
    }

    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) libItemMan->getLibitems().size()) {
        return false;
    }
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    if (libitem->loadLocalParams(jsonObject["libitem"].toObject()) != 0) {
        emit dataChanged(index, index);
        return false;
    }
    if (!media || media->loadLocalParams((jsonObject["media"]["media"]).toObject()) != 0 || media->loadSubclassParams(jsonObject["media"]["subclass_params"].toObject()) != 0) {
        emit dataChanged(index, index);
        return false;
    }
    emit dataChanged(index, index);
    return true;
    
}

QJsonObject LibItemTableModel::getDefaultSchema(QString mediaType) const {
    QJsonObject defaultSchema;
    defaultSchema.insert("type", "object");
    QJsonObject properties;
    properties.insert("libitem", Libitem::getSchema());
    mediaType = mediaType.toLower();
    if (mediaType == "audio") {
        properties.insert("media", Audio::getSchema());
    } else if (mediaType == "video") {
        properties.insert("media", Video::getSchema());
    } else if (mediaType == "text") {
        properties.insert("media", Text::getSchema());
    } else {
        properties.insert("media", Media::getSchema());
    }
    defaultSchema.insert("properties", properties);
    return defaultSchema;
}

QJsonObject LibItemTableModel::getDefaultJsonObject(QString mediaType) const {
    QJsonObject defaultSchema = getDefaultSchema(mediaType);
    QJsonObject defaultJsonObject = createDefaultJsonFromSchema(defaultSchema);
    if (defaultJsonObject.contains("libitem")) {
        QJsonObject libitemObject = defaultJsonObject["libitem"].toObject();
        libitemObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(libItemMan->getNextId()));
        defaultJsonObject["libitem"] = libitemObject;
    }
    if (defaultJsonObject.contains("media")) {
        QJsonObject mediaObject = defaultJsonObject["media"].toObject();
        if (mediaObject.contains("subclass_params") && mediaObject["subclass_params"].toObject().isEmpty()) {
            mediaObject.remove("subclass_params");
        }

        QJsonObject baseObject = mediaObject["media"].toObject();
        baseObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(mediaMan->getNextId()));
        mediaObject["media"] = baseObject;
        defaultJsonObject["media"] = mediaObject;
    }
    return defaultJsonObject;
}

QJsonObject LibItemTableModel::getDefaultSchema() const {
    return getDefaultSchema("media");
}

QJsonObject LibItemTableModel::getDefaultJsonObject() const {
    return getDefaultJsonObject("media");
}

void LibItemTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}