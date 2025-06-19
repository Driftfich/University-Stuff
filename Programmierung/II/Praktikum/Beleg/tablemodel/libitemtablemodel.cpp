
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

#include "returns.h"

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
    // check if the media item behind the libitem is referenced by any other libitem
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    if (libitem == nullptr) {
        return false;
    }
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());
    if (media == nullptr) {
        return false;
    }
    // Check if the media item is referenced by any other libitem
    bool hasOtherReferences = false;
    for (const auto& item : libItemMan->getLibitems()) {
        if (item->getMediaId() == media->getId() && item->getId() != libitem->getId()) {
            hasOtherReferences = true;
            break;
        }
    }

    if (!hasOtherReferences) {
        // remove the media item if it is not referenced by any other libitem
        if (mediaMan->removeMedia(media->getId()) != 0) {
            return false; // Failed to remove media item
        }
    }
    beginRemoveRows(parent, row, row);
    libItemMan->removeLibitem(row);
    endRemoveRows();
    return true;
}

bool LibItemTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row + count > libItemMan->getLibitems().size()) {
        return false;
    }
    // beginRemoveRows(parent, row, row + count - 1);
    // for (int i = 0; i < count; ++i) {
    //     libItemMan->removeLibitem(row);
    // }
    // endRemoveRows();
    for (int i = 0; i < count; ++i) {
        if (!removeRow(row, parent)) {
            return false; // If any row removal fails, return false
        }
    }
    
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
        properties.insert("media", media->getSchemaByType());
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
    // std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    // std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    // if (libitem->loadLocalParams(jsonObject["libitem"].toObject()) != 0) {
    //     emit dataChanged(index, index);
    //     return false;
    // }
    // if (!media || media->loadLocalParams((jsonObject["media"]["media"]).toObject()) != 0 || media->loadSubclassParams(jsonObject["media"]["subclass_params"].toObject()) != 0) {
    //     emit dataChanged(index, index);
    //     return false;
    // }
    if (!updateLibitemFromJsonObject(jsonObject) || !updateMediaFromJsonObject(jsonObject)) {
        return false;
    }
    emit dataChanged(index, index);
    return true;
    
}

int LibItemTableModel::updateMediaFromJsonObject(const QJsonObject& jsonObject) {
    unsigned long mediaId = jsonObject["media"].toObject()["media"].toObject()["id"].toVariant().toULongLong();
    std::shared_ptr<Media> media = mediaMan->getMedia(mediaId);
    if (!media) {
        qDebug() << "Cannot update media from JSON object, media not found with ID:" << mediaId;
        return -1;
    }
    if (media->loadLocalParams(jsonObject["media"]["media"].toObject()) != 0 || media->loadSubclassParams(jsonObject["media"]["subclass_params"].toObject()) != 0) {
        qDebug() << "Failed to update media from JSON object, issues with loading parameters";
        return -2;
    }
    return 0;
}

bool LibItemTableModel::updateLibitemFromJsonObject(const QJsonObject& jsonObject) {
    unsigned long libitemId = jsonObject["libitem"].toObject()["id"].toVariant().toULongLong();
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(libitemId);
    if (!libitem) {
        qDebug() << "Cannot update libitem from JSON object, libitem not found with ID:" << libitemId;
        return false;
    }
    if (libitem->loadLocalParams(jsonObject["libitem"].toObject()) != 0) {
        return false;
    }
    return true;
}

Result LibItemTableModel::saveFromJsonObject(const QJsonObject& jsonObject) {
    if (!jsonObject.contains("libitem") || !jsonObject.contains("media")) {
        qDebug() << "JSON object does not contain required keys 'libitem' or 'media'";
        return Result::Error("Invalid JSON object");
    }

    // try to update the media item. When the return value is -1 a new media item needs to be created
    int mediaUpdateResult = updateMediaFromJsonObject(jsonObject);
    qDebug() << "Media update result:" << mediaUpdateResult;
    if (mediaUpdateResult == -1) {
        // create a new media item
        QJsonObject mediaJson = jsonObject["media"].toObject();
        std::shared_ptr<Media> newMedia = Media::MediaFactory(mediaJson);
        if (!newMedia) {
            qDebug() << "Failed to create new media item from JSON object";
            return Result::Error("Failed to create new media item");
        }
        mediaMan->addMedia(newMedia);
    }

    // libitem is always new
    QJsonObject libitemJson = jsonObject["libitem"].toObject();
    std::shared_ptr<Libitem> newLibitem = Libitem::LibitemFactory(libitemJson);
    if (!newLibitem) {
        qDebug() << "Failed to create new libitem from JSON object";
        return Result::Error("Failed to create new libitem");
    }
    libItemMan->addLibitem(newLibitem);

    // emit dataChanged signal to update the view
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    emit layoutChanged(); // Notify that the layout has changed
    return Result::Success();
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

    // make the subclass_type readonly false
    QJsonObject mediaSchema = properties["media"].toObject();
    QJsonObject mediaProperties = mediaSchema["properties"].toObject();
    QJsonObject subclassType = mediaProperties["subclass_type"].toObject();
    subclassType["readonly"] = false; // make it editable
    mediaProperties["subclass_type"] = subclassType;
    mediaSchema["properties"] = mediaProperties;
    properties["media"] = mediaSchema;
    // qDebug() << properties;
    defaultSchema.insert("properties", properties);
    return defaultSchema;
}

QJsonObject LibItemTableModel::getDefaultSchema() const {
    return getDefaultSchema("media");
}

QJsonObject LibItemTableModel::getDefaultJsonObject(QString mediaType) const {
    QJsonObject defaultSchema = getDefaultSchema(mediaType);
    QJsonObject defaultJsonObject = createDefaultJsonFromSchema(defaultSchema);
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
    if (defaultJsonObject.contains("libitem")) {
        QJsonObject libitemObject = defaultJsonObject["libitem"].toObject();
        libitemObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(libItemMan->getNextId()));
        libitemObject["media_id"] = QJsonValue::fromVariant(static_cast<quint64>(mediaMan->getNextId()));
        defaultJsonObject["libitem"] = libitemObject;
    }
    return defaultJsonObject;
}

QJsonObject LibItemTableModel::getDefaultJsonObject() const {
    return getDefaultJsonObject("media");
}

void LibItemTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}